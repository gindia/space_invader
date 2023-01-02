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
///

#pragma once

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

#define DEFAULT_FONT_SIZE 32

typedef struct Renderer Renderer;

typedef size_t AtlasIndex;

typedef struct {
  AtlasIndex atlas_index; // the atlas index in the renderer.

#define SPRITE_ANIMATION_MAX_LENGTH 16
  u8     sprite_index  [SPRITE_ANIMATION_MAX_LENGTH];
  f32    duration_in_ms[SPRITE_ANIMATION_MAX_LENGTH];

  u32    taint; // hex color multiplyer to the texture
                // 0xRRGGBBAA R:red G:green B:blue A:alpha

  size_t animation_len; // how many frames in the animation.
  size_t current_frame;

  f32    elapsed_time;  // DO NOT SET:: tracks time to switch between frames
}Animation;

REND_DEF Renderer * rend_init(void);
REND_DEF void       rend_tick(Renderer *);
REND_DEF void       rend_drop(Renderer *);

// replaces the font used for rendering.
REND_DEF void       rend_font_load (Renderer *renderer, const u8 *ttf_raw, size_t font_rendering_size);
REND_DEF AtlasIndex rend_atlas_load(Renderer *renderer, const u8 *raw_image_buffer, size_t buffer_size, size_t rows, size_t columns, f32 sprite_size);

REND_DEF void rend_animation_load(const u8 *buffer, size_t buffer_size, AtlasIndex, Animation *out);
REND_DEF void rend_animaion_tick (Animation *, f32 delta_time);

REND_DEF void rend_text_draw       (Renderer *, quad, u32 color, const char *fmt, ...);
REND_DEF void rend_animation_draw  (Renderer *, Animation *, quad);
REND_DEF void rend_line_draw       (Renderer *, vec2 from, vec2 to, f32 width, u32 color);
REND_DEF void rend_quad_draw_filled(Renderer *, quad, u32 color);
REND_DEF void rend_quad_draw_empty (Renderer *, quad, f32 width, u32 color);

// RENDERER_IMPLEMENTATION {{{
#ifdef RENDERER_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <GLES3/gl3.h>

#define U32_RGBA_COMMA(hex)   \
    ((f32)((hex & 0xFF000000) >> 24)) / 255.0f, \
    ((f32)((hex & 0x00FF0000) >> 16)) / 255.0f, \
    ((f32)((hex & 0x0000FF00) >> 8 )) / 255.0f, \
    ((f32) (hex & 0x000000FF)       ) / 255.0f

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
  f32     sprite_size;
}Atlas;

struct Renderer {
  Shader  shader[SHADER_COUNT];
  Font    font;
  Texture font_texture;

#define REDNERER_LOADED_ATLAS_CAPACITY 32L
  Atlas  atlas[REDNERER_LOADED_ATLAS_CAPACITY];
  size_t atlas_len;

  VAO vao;
  VBO vbo;
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

internal quad
rend__atlas_sprite_coords(Atlas *atlas, size_t index)
{
  f32 iw, ih;
  size_t x, y;
  quad out;

  iw = atlas->sprite_size/(f32)atlas->texture.width;
  ih = atlas->sprite_size/(f32)atlas->texture.height;

  y = index / atlas->columns;
  x = index % atlas->columns;

  out.min.x = x * iw;
  out.min.y = y * ih;
  out.max.x = (x + 1) * iw;
  out.max.y = (y + 1) * ih;

  return out;
}

internal b32
rend__font_parse(const u8 *ttf_raw, size_t font_rendering_size, Font *font)
{
  Assert(ttf_raw && "passing null as raw");
  Assert(font && "pleas pass allocated font before calling util_font_pars(..)!");
  font->img_width    = 1024;
  font->img_height   = 1024;
  font->img_channels = 1;
  font->font_size  = font_rendering_size;
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
rend__font_get_quad(Font *font, char c_to_display, float *xpos, float *ypos, stbtt_aligned_quad *q)
{
  Assert(font);
  stbtt_GetPackedQuad((stbtt_packedchar*)font->packed_char, font->img_width, font->img_height,
                      (c_to_display - ' '),     // character to display
                      xpos, ypos,               // pointers to current position in screen pixel space
                      (stbtt_aligned_quad*)q,   // output: quad to draw
                      1);
}


Renderer *
rend_init(void)
{
  Renderer *renderer;
  char *shader_src_font, *shader_src_atlas;
  u8 *ttf_raw;
  size_t src_len_font, src_len_atlas, ttf_size;
  b32 success;

  renderer = malloc(sizeof(*renderer));
  memset(renderer, 0, sizeof(*renderer));

  shader_src_font  = (char*)plt_bin_read_from_desk("res/shader_font.glsl",  &src_len_font);
  shader_src_atlas = (char*)plt_bin_read_from_desk("res/shader_atlas.glsl", &src_len_atlas);

  Assert(shader_src_font);
  Assert(shader_src_atlas);

  renderer->shader[SHADER_FONT]  = rend__shader_init(shader_src_font,  src_len_font);
  renderer->shader[SHADER_ATLAS] = rend__shader_init(shader_src_atlas, src_len_atlas);

  Assert(renderer->shader[SHADER_FONT]);
  Assert(renderer->shader[SHADER_ATLAS]);

  ttf_raw = plt_bin_read_from_desk("res/Minimal5x7.ttf", &ttf_size);
  Assert(ttf_raw);

  rend_font_load(renderer, ttf_raw, DEFAULT_FONT_SIZE);

  glGenVertexArrays(1, &renderer->vao);
  glGenBuffers(1, &renderer->vbo);

  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, (6 * 4 * sizeof(GLfloat)), NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    // pos uv
    // 2   2
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(GL_FLOAT)), (void*) 0);
    glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  plt_bin_free(shader_src_font);
  plt_bin_free(shader_src_atlas);
  plt_bin_free(ttf_raw);

  return renderer;
}

void
rend_tick(Renderer *)
{
  i32 width, height;

  plt_gl_swap_buffers();

  plt_window_size(&width, &height);
  glViewport(0, 0, width, height);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
rend_drop(Renderer *renderer)
{
  rend__font_free(&renderer->font);
  free(renderer);
}

// replaces the font used for rendering.
void
rend_font_load(Renderer *renderer, const u8 *ttf_raw, size_t font_rendering_size)
{
  Assert(renderer);
  Assert(ttf_raw);

  rend__font_free(&renderer->font);
  rend__font_parse(ttf_raw, font_rendering_size, &renderer->font);

  if (glIsTexture(renderer->font_texture.id)) {
    glDeleteTextures(1, &renderer->font_texture.id);
  }

  renderer->font_texture = rend__texture_init(renderer->font.img_buffer, renderer->font.img_width,
      renderer->font.img_height, renderer->font.img_channels);
}

AtlasIndex
rend_atlas_load(Renderer* renderer, const u8 *raw_image_buffer, size_t buffer_size, size_t rows, size_t columns, f32 sprite_size)
{
  size_t index;

  Assert(renderer);
  Assert(raw_image_buffer && buffer_size);
  Assert(renderer->atlas_len < REDNERER_LOADED_ATLAS_CAPACITY);

  index = renderer->atlas_len;

  renderer->atlas[index].texture     = rend__texture_load(raw_image_buffer, buffer_size);
  renderer->atlas[index].rows        = rows;
  renderer->atlas[index].columns     = columns;
  renderer->atlas[index].sprite_size = sprite_size;

  renderer->atlas_len++;

  return index;
}

void
rend_animation_load(const u8 *buffer, size_t buffer_size, AtlasIndex, Animation *out)
{
  Assert(0 && "Not Implemented Yet");
}

void
rend_animaion_tick(Animation *animation, f32 delta_time)
{
  animation->elapsed_time += delta_time;
  if (animation->elapsed_time >= animation->duration_in_ms[animation->current_frame]) {
    animation->elapsed_time = 0.0;
    if (animation->current_frame < (animation->animation_len - 1)) {
      animation->current_frame ++;
    } else {
      animation->current_frame = 0;
    }
  }
}

#define DRAWING_TEXT_CAPACITY 2048

void
rend_text_draw(Renderer *renderer, quad q, u32 color, const char *fmt, ...)
{
  char buffer[DRAWING_TEXT_CAPACITY];
  size_t buffer_len;
  va_list va;
  f32 xpos, ypos;
  stbtt_aligned_quad stb_q;
  i32 window_width, window_height;
  GLint u_space_matrix, u_taint, u_tex0;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  plt_window_size(&window_width, &window_height);

  va_start(va, fmt);
  buffer_len = vsnprintf(buffer, DRAWING_TEXT_CAPACITY, fmt, va);
  va_end(va);

  xpos = q.min.x;
  ypos = q.min.y  + (renderer->font.font_size / 2);

  glUseProgram(renderer->shader[SHADER_FONT]);
  u_space_matrix = glGetUniformLocation(renderer->shader[SHADER_FONT], "u_space_matrix");
  u_taint        = glGetUniformLocation(renderer->shader[SHADER_FONT], "u_taint");
  u_tex0         = glGetUniformLocation(renderer->shader[SHADER_FONT], "u_tex0");

  if (-1 != u_space_matrix) {
    mat4 space_matrix;
    space_matrix = mat4_ortho(0.0, (f32)window_width, (f32)window_height, 0.0, -0.1, 0.1);
    glUniformMatrix4fv(u_space_matrix, 1, GL_FALSE, &space_matrix.m[0][0]);
  }

  if (-1 != u_taint) {
    glUniform4f(u_taint, U32_RGBA_COMMA(color));
  }

  if (-1 != u_tex0) {
    glUniform1i(u_tex0, 0);
  }

  glBindVertexArray(renderer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, renderer->font_texture.id);

  for ( size_t i = 0
      ; i < buffer_len
      ; i ++ )
  {
    rend__font_get_quad(&renderer->font, buffer[i], &xpos, &ypos, &stb_q);
    if (xpos > q.max.x) break;

    f32 vertices[] = {
        stb_q.x1, stb_q.y1, stb_q.s1, stb_q.t1, // 0
        stb_q.x1, stb_q.y0, stb_q.s1, stb_q.t0, // 1
        stb_q.x0, stb_q.y1, stb_q.s0, stb_q.t1, // 2
        stb_q.x1, stb_q.y0, stb_q.s1, stb_q.t0, // 3
        stb_q.x0, stb_q.y0, stb_q.s0, stb_q.t0, // 4
        stb_q.x0, stb_q.y1, stb_q.s0, stb_q.t1, // 5
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * 6, vertices, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
}

void
rend_animation_draw(Renderer *renderer, Animation *animation, quad q)
{
  f32 x0, x1, y0, y1, s0, s1, t0, t1;
  i32 window_width, window_height;
  GLint u_space_matrix, u_model, u_taint, u_tex0, u_use_texture;
  Atlas *atlas;
  quad sprite_coords;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  plt_window_size(&window_width, &window_height);

  glUseProgram(renderer->shader[SHADER_ATLAS]);
  u_space_matrix = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_space_matrix");
  u_model        = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_model");
  u_taint        = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_taint");
  u_tex0         = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_tex0");
  u_use_texture  = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_use_texture");

  if (-1 != u_space_matrix) {
    mat4 space_matrix;
    space_matrix = mat4_ortho(0.0, (f32)window_width, (f32)window_height, 0.0, -0.1, 0.1);
    glUniformMatrix4fv(u_space_matrix, 1, GL_FALSE, &space_matrix.m[0][0]);
  }

  if (-1 != u_model) {
    mat4 model;
    model = mat4_identity();
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model.m[0][0]);
  }

  if (-1 != u_taint) {
    glUniform4f(u_taint, U32_RGBA_COMMA(animation->taint));
  }

  if (-1 != u_tex0) {
    glUniform1i(u_tex0, 0);
  }

  if (-1 != u_use_texture) {
    glUniform1i(u_use_texture, 1);
  }

  Assert(animation->atlas_index < renderer->atlas_len && "incorrect atlas index [out of bounds || not loaded]");
  atlas = &renderer->atlas[animation->atlas_index];

  sprite_coords = rend__atlas_sprite_coords(atlas, animation->sprite_index[animation->current_frame]);

  glBindVertexArray(renderer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

  x0 = q.min.x; x1 = q.max.x; y0 = q.min.y; y1 = q.max.y;
  s0 = sprite_coords.min.x;
  s1 = sprite_coords.max.x;
  t0 = sprite_coords.min.y;
  t1 = sprite_coords.max.y;

  f32 vertices[] = {
    x1, y1, s1, t1, // 0
    x1, y0, s1, t0, // 1
    x0, y1, s0, t1, // 2
    x1, y0, s1, t0, // 3
    x0, y0, s0, t0, // 4
    x0, y1, s0, t1, // 5
  };

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas->texture.id);

  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * 6, vertices, GL_DYNAMIC_DRAW);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glDisable(GL_BLEND);

}

void rend_line_draw       (Renderer *, vec2 from, vec2 to, f32 width, u32 color);

void
rend_quad_draw_filled(Renderer *renderer, quad q, u32 color)
{
  f32 x0, x1, y0, y1, s0, s1, t0, t1;
  i32 window_width, window_height;

  plt_window_size(&window_width, &window_height);

  glUseProgram(renderer->shader[SHADER_ATLAS]);
  GLint u_space_matrix = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_space_matrix");
  GLint u_model        = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_model");
  GLint u_taint        = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_taint");
  GLint u_tex0         = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_tex0");
  GLint u_use_texture  = glGetUniformLocation(renderer->shader[SHADER_ATLAS], "u_use_texture");

  if (-1 != u_space_matrix) {
    mat4 space_matrix;
    space_matrix = mat4_ortho(0.0, (f32)window_width, (f32)window_height, 0.0, -0.1, 0.1);
    glUniformMatrix4fv(u_space_matrix, 1, GL_FALSE, &space_matrix.m[0][0]);
  }

  if (-1 != u_model) {
    mat4 model;
    model = mat4_identity();
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model.m[0][0]);
  }

  if (-1 != u_taint) {
    glUniform4f(u_taint, U32_RGBA_COMMA(color));
  }

  if (-1 != u_tex0) {
    glUniform1i(u_tex0, 0);
  }

  if (-1 != u_use_texture) {
    glUniform1i(u_use_texture, 0);
  }

  glBindVertexArray(renderer->vao);
  glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

  x0 = q.min.x; x1 = q.max.x; y0 = q.min.y; y1 = q.max.y;
  s0 = 0.0; s1 = 1.0; t0 = 0.0; t1 = 1.0;

  f32 vertices[] = {
    x1, y1, s1, t1, // 0
    x1, y0, s1, t0, // 1
    x0, y1, s0, t1, // 2
    x1, y0, s1, t0, // 3
    x0, y0, s0, t0, // 4
    x0, y1, s0, t1, // 5
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * 6, vertices, GL_DYNAMIC_DRAW);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void rend_quad_draw_empty (Renderer *, quad, f32 width, u32 color);


#endif // RENDERER_IMPLEMENTATION
// }}} RENDERER_IMPLEMENTATION

// vim: sw=2 fdm=marker
