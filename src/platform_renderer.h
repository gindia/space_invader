///
/// Platform renderer layer for 2D Sprites
///
/// ----------------------------------
/// How to ues
///   #define RENDERER_IMPLEMENTATION
///   #include ...
///
///   * don't forget to link agnist -lGLESv2
///   * make sure that platform.h exists at the same dir.
///   * make sure that stb_image.h, stb_truetype.h and stb_rect_pack.h exists at the same dir.
///

#include "platform.h"

#ifdef REND_STATIC
#   define REND_DEF internal
#else
#   ifdef __cplusplus
#       define REND_DEF extern "C"
#   else
#       define REND_DEF extern
#   endif
#endif

typedef struct Renderer        Renderer;
typedef struct SpriteSheet     SpriteSheet;

typedef struct {
  SpriteSheet *sheet; // make sure that the SpriteSheet lives as long as or more than the lifetime of
                      // the SpriteAnimation.
#define SPRITE_ANIMATION_MAX_LENGTH 16
  u8     sprite_index[SPRITE_ANIMATION_MAX_LENGTH];
  size_t animation_len; // how many frames in the animation.

}SpriteAnimation;

REND_DEF Renderer *    rend_init(void);
REND_DEF void          rend_tick(Renderer *);
REND_DEF void          rend_drop(Renderer *);

REND_DEF void          rend_use_font(Renderer *, const u8 *ttf_raw_buffer, size_t size);

REND_DEF SpriteSheet * sprite_sheet_init(const u8 *raw_image_buffer, size_t size);
REND_DEF void          sprite_sheet_drop(SpriteSheet *);

// RENDERER_IMPLEMENTATION {{{
#ifdef RENDERER_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <GLES3/gl3.h>

typedef GLuint Shader;
typedef GLuint Texture;
typedef GLint  Uniform;

typedef struct {
    unsigned char *img_buffer;
    int img_width, img_height, img_channels;

    int font_size;

#define NUMBER_OF_PACKED_CHARS ('~'-' ') // ansi
    stbtt_packedchar packed_char[NUMBER_OF_PACKED_CHARS];
}Font;

struct SpriteSheet {
  Texture texture;
};

struct Renderer {
  Shader shader;
};

internal Shader rend_shader_init(const char *src, size_t len);
internal void   rend_shader_drop(Shader);

internal Shader rend_texture_init(const u8 *buffer, size_t size);
internal void   rend_texture_drop(Texture);

#define GLSL_VERSION            "#version 300 es\n"
#define DEFINE_SHADER_VERTEX    "#define VERTEX_SHADER\n"
#define DEFINE_SHADER_FRAGMENT  "#define FRAGMENT_SHADER\n"

internal b32
font_parse(const u8 *ttf_raw, i32 font_size, Font *font)
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
font_free(Font *font)
{
    Assert(font && "passing invalid font to free");
    if (font->img_buffer != NULL)
        free(font->img_buffer);
}

internal void
font_get_quad(Font *font, char c_to_display, float *xpos, float *ypos, stbtt_aligned_quad *quad)
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
