///
/// Platform renderer layer for 2D Sprites
///
/// ----------------------------------
/// How to ues
///   #define RENDERER_IMPLEMENTATION
///   #include ...
///
/// Dependencies
///   * OpenGL ES [link agnist -lGLESv2]
///   * make sure that platform.h and vmath.h exists at the same dir.
///   * make sure that stb_image.h, stb_truetype.h and stb_rect_pack.h exists at the same dir.
///   * make sure that xxhash.h exists and linked.
///

#include "platform.h"
#include "vmath.h"

#ifdef REND_STATIC
#   define REND_DEF internal
#else
#   ifdef __cplusplus
#       define REND_DEF extern "C"
#   else
#       define REND_DEF extern
#   endif
#endif

typedef struct Renderer Renderer;

typedef u64 AtlasID;

typedef struct {
  AtlasID atlas_id;

#define SPRITE_ANIMATION_MAX_LENGTH 16
  u8     sprite_index  [SPRITE_ANIMATION_MAX_LENGTH];
  f32    duration_in_ms[SPRITE_ANIMATION_MAX_LENGTH];
  size_t animation_len; // how many frames in the animation.
  size_t current_frame;

  u32    taint; // hex color multiplyer to the texture
                // 0xRRGGBBAA R:red G:green B:blue A:alpha

  f32    elapsed_time;  // DO NOT SET:: tracks time to switch between frames
}Animation;

REND_DEF Renderer *    rend_init(const u8 *ttf_raw_buffer, size_t ttf_raw_buffer_size);
REND_DEF void          rend_tick(Renderer *);
REND_DEF void          rend_drop(Renderer *);

// replaces the font used for rendering.
REND_DEF void          rend_load_font (Renderer *, const u8 *ttf_raw_buffer, size_t size);
REND_DEF AtlasID       rend_load_atlas(const u8 *raw_image_buffer, size_t size, size_t rows, size_t columns);

REND_DEF void animation_load(const u8 *buffer, size_t buffer_size, AtlasID *, Animation *);
REND_DEF void animation_tick(Animation *, f32 delta_time);

REND_DEF void rend_draw_text       (Renderer *, quad, const char *fmt, ...);
REND_DEF void rend_draw_animation  (Renderer *, Animation *, quad);
REND_DEF void rend_draw_line       (Renderer *, vec2 from, vec2 to, f32 width, u32 color);
REND_DEF void rend_draw_quad_filled(Renderer *, quad, u32 color);
REND_DEF void rend_draw_quad_empty (Renderer *, quad, f32 width, u32 color);

// RENDERER_IMPLEMENTATION {{{
#ifdef RENDERER_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "xxhash.h"

#include <GLES3/gl3.h>

typedef GLuint Shader;
typedef GLint  Uniform;
typedef GLuint VBO;
typedef GLuint VAO;

enum {
  SHADER_FONT,
  SHADER_ATLAS,

  SHADER_COUNT,
};

typedef struct {
  GLuint id;
  i32 width;
  i32 height;
  i32 channels;
}Texture;

typedef struct {
    unsigned char *img_buffer;
    int img_width, img_height, img_channels;

    int font_size;

#define NUMBER_OF_PACKED_CHARS ('~'-' ') // ansi
    stbtt_packedchar packed_char[NUMBER_OF_PACKED_CHARS];
}Font;

typedef struct {
  Texture texture;
  size_t  rows;
  size_t  columns;
}Atlas;

struct Renderer {
  Shader shader[SHADER_COUNT];
  Font   font;
#define RENDERER_MAX_LOADED_ATLASES 8
  Atlas   atlas   [RENDERER_MAX_LOADED_ATLASES];
  AtlasID atlas_id[RENDERER_MAX_LOADED_ATLASES];
  size_t atlases_size;
};

#define GLSL_VERSION            "#version 300 es\n"
#define DEFINE_SHADER_VERTEX    "#define VERTEX_SHADER\n"
#define DEFINE_SHADER_FRAGMENT  "#define FRAGMENT_SHADER\n"

/// returns 0 if no error found
internal const char *
rend__gles_error_str(void) {
  GLenum err = glGetError();
  switch (err) {
    case GL_NO_ERROR:
      return 0;
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    UNREACHABLE_CASE;
  }
}

internal Shader
rend__shader_init(const char *src, size_t len)
{
  GLuint program  = glCreateProgram();
  GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar * v_src[3] = {GLSL_VERSION, DEFINE_SHADER_VERTEX,   src};
  const GLchar * f_src[3] = {GLSL_VERSION, DEFINE_SHADER_FRAGMENT, src};
  GLint v_len[3] = {strlen(GLSL_VERSION), strlen(DEFINE_SHADER_VERTEX),   len};
  GLint f_len[3] = {strlen(GLSL_VERSION), strlen(DEFINE_SHADER_FRAGMENT), len};

  glShaderSource(v_shader, 3, v_src, v_len);
  glShaderSource(f_shader, 3, f_src, f_len);

  b32 success = 0;

  glCompileShader(v_shader);
  glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);

  if (success) {
    glCompileShader(f_shader);
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);

    if (success) {
      glAttachShader(program, v_shader);
      glAttachShader(program, f_shader);
      glLinkProgram(program);
      glGetProgramiv(program, GL_LINK_STATUS, &success);

      if (success) {
        glDetachShader(program, v_shader);
        glDetachShader(program, f_shader);
        return program;

      } else {
        // log error
        char error_log[1024];
        glGetProgramInfoLog(program, 1024, NULL, &error_log[0]);
        plt_log_error("Shader Program :: %s", error_log);
        glDeleteProgram(program);
      }
    } else {
      // log error
      char error_log[1024];
      glGetShaderInfoLog(f_shader, 1024, NULL, &error_log[0]);
      plt_log_error("Shader Fragment :: %s", error_log);
      glDeleteShader(f_shader);
      glDeleteShader(v_shader);
    }
  } else {
    // log error
    char error_log[1024];
    glGetShaderInfoLog(v_shader, 1024, NULL, &error_log[0]);
    plt_log_error("Shader Vertex :: %s", error_log);
    glDeleteShader(v_shader);
  }
  return 0;
}

internal Texture
rend__texture_init(const u8 *pixels, i32 width, i32 height, i32 channels)
{
  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  switch (channels) {
    case 1: {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    }break;

    case 3: {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }break;

    case 4: {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }break;

    // { Passing image with unsupported number of channels.
    UNREACHABLE_CASE;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  Texture t;
  t.id       = texture;
  t.width    = width;
  t.height   = height;
  t.channels = channels;
  return t;
}

internal Texture
rend__texture_load(const u8 *buffer, size_t size)
{
  Assert(buffer && size && "passing a null buffer or an empty one.");

  i32 width, height, channels;
  u8 *pixels = stbi_load_from_memory(buffer, size, &width, &height, &channels, 0);

  Texture t = rend__texture_init(pixels, width, height, channels);

  stbi_image_free(pixels);

  return t;
}

internal b32
rend__font_parse(const u8 *ttf_raw, i32 font_size, Font *font)
{
  Assert(ttf_raw && "passing null as raw");
  Assert(font && "pleas pass allocated font before calling util_font_pars(..)!");
  font->img_width    = 1024;
  font->img_height   = 1024;
  font->img_channels = 1;
  font->font_size  = font_size;
  font->img_buffer = malloc(font->img_width * font->img_height * font->img_channels);
  Assert(font->img_buffer && "Failed to allocate mem for pixels.");

  b32 success = 0;
  stbtt_pack_context spc;
  success = stbtt_PackBegin(&spc, font->img_buffer, font->img_width, font->img_height, font->img_width, 1, NULL);
  Assert(success);

  stbtt_PackSetOversampling(&spc, 8, 8);

  success = stbtt_PackFontRange(&spc, ttf_raw, 0, STBTT_POINT_SIZE(font->font_size),
          ' ', NUMBER_OF_PACKED_CHARS, (stbtt_packedchar *)&font->packed_char[0]);
  Assert(success);

  stbtt_PackEnd(&spc);

  return success;
}

internal void
rend__font_free(Font *font)
{
  Assert(font && "passing invalid font to free");
  if (font->img_buffer != NULL)
      free(font->img_buffer);
}

internal void
rend__font_get_quad(Font *font, char c_to_display, float *xpos, float *ypos, stbtt_aligned_quad *quad)
{
  Assert(font);
  stbtt_GetPackedQuad((stbtt_packedchar*)font->packed_char, font->img_width, font->img_height,
                      (c_to_display - ' '),     // character to display
                      xpos, ypos,               // pointers to current position in screen pixel space
                      (stbtt_aligned_quad*)quad,// output: quad to draw
                      1);
}


#endif // RENDERER_IMPLEMENTATION
// }}} RENDERER_IMPLEMENTATION

// vim: sw=2 fdm=marker
