///
/// Platform layer (SDL2 wrapper)
///
/// ----------------------------------
/// How to ues
///   #define PLT_IMPLEMENTATION
///   #include ...
///
///   and don't forget to link agnist -lSDL2 -lSDL2_mixer -lGLESv2
///
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int32_t  b32;

typedef float    f32;
typedef double   f64;

#define internal static
#define global   static
#define local    static

#ifdef PLT_STATIC
#   define PLT_DEF internal
#else
#   ifdef __cplusplus
#       define PLT_DEF extern "C"
#   else
#       define PLT_DEF extern
#   endif
#endif

typedef size_t Key;
typedef size_t MouseKey;

// enum KEY_* {{{
// this enum is just a straight up copy from `SDL2/Scancode.h` with shortened up names for convince.
enum {
  KEY_UNKNOWN = 0,

  /**
    *  \name Usage page 0x07
    *
    *  These values are from usage page 0x07 (USB keyboard page).
    */
  /* @{ */

  KEY_A = 4,
  KEY_B = 5,
  KEY_C = 6,
  KEY_D = 7,
  KEY_E = 8,
  KEY_F = 9,
  KEY_G = 10,
  KEY_H = 11,
  KEY_I = 12,
  KEY_J = 13,
  KEY_K = 14,
  KEY_L = 15,
  KEY_M = 16,
  KEY_N = 17,
  KEY_O = 18,
  KEY_P = 19,
  KEY_Q = 20,
  KEY_R = 21,
  KEY_S = 22,
  KEY_T = 23,
  KEY_U = 24,
  KEY_V = 25,
  KEY_W = 26,
  KEY_X = 27,
  KEY_Y = 28,
  KEY_Z = 29,

  KEY_1 = 30,
  KEY_2 = 31,
  KEY_3 = 32,
  KEY_4 = 33,
  KEY_5 = 34,
  KEY_6 = 35,
  KEY_7 = 36,
  KEY_8 = 37,
  KEY_9 = 38,
  KEY_0 = 39,

  KEY_RETURN = 40,
  KEY_ESCAPE = 41,
  KEY_BACKSPACE = 42,
  KEY_TAB = 43,
  KEY_SPACE = 44,

  KEY_MINUS = 45,
  KEY_EQUALS = 46,
  KEY_LEFTBRACKET = 47,
  KEY_RIGHTBRACKET = 48,
  KEY_BACKSLASH = 49, /**< Located at the lower left of the return
                                *   key on ISO keyboards and at the right end
                                *   of the QWERTY row on ANSI keyboards.
                                *   Produces REVERSE SOLIDUS (backslash) and
                                *   VERTICAL LINE in a US layout, REVERSE
                                *   SOLIDUS and VERTICAL LINE in a UK Mac
                                *   layout, NUMBER SIGN and TILDE in a UK
                                *   Windows layout, DOLLAR SIGN and POUND SIGN
                                *   in a Swiss German layout, NUMBER SIGN and
                                *   APOSTROPHE in a German layout, GRAVE
                                *   ACCENT and POUND SIGN in a French Mac
                                *   layout, and ASTERISK and MICRO SIGN in a
                                *   French Windows layout.
                                */
  KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                *   instead of 49 for the same key, but all
                                *   OSes I've seen treat the two codes
                                *   identically. So, as an implementor, unless
                                *   your keyboard generates both of those
                                *   codes and your OS treats them differently,
                                *   you should generate KEY_BACKSLASH
                                *   instead of this code. As a user, you
                                *   should not rely on this code because SDL
                                *   will never generate it with most (all?)
                                *   keyboards.
                                */
  KEY_SEMICOLON = 51,
  KEY_APOSTROPHE = 52,
  KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                            *   and ISO keyboards). Produces GRAVE ACCENT and
                            *   TILDE in a US Windows layout and in US and UK
                            *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                            *   and NOT SIGN in a UK Windows layout, SECTION
                            *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                            *   layouts on ISO keyboards, SECTION SIGN and
                            *   DEGREE SIGN in a Swiss German layout (Mac:
                            *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                            *   DEGREE SIGN in a German layout (Mac: only on
                            *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                            *   French Windows layout, COMMERCIAL AT and
                            *   NUMBER SIGN in a French Mac layout on ISO
                            *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                            *   SIGN in a Swiss German, German, or French Mac
                            *   layout on ANSI keyboards.
                            */
  KEY_COMMA = 54,
  KEY_PERIOD = 55,
  KEY_SLASH = 56,

  KEY_CAPSLOCK = 57,

  KEY_F1 = 58,
  KEY_F2 = 59,
  KEY_F3 = 60,
  KEY_F4 = 61,
  KEY_F5 = 62,
  KEY_F6 = 63,
  KEY_F7 = 64,
  KEY_F8 = 65,
  KEY_F9 = 66,
  KEY_F10 = 67,
  KEY_F11 = 68,
  KEY_F12 = 69,

  KEY_PRINTSCREEN = 70,
  KEY_SCROLLLOCK = 71,
  KEY_PAUSE = 72,
  KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                  does send code 73, not 117) */
  KEY_HOME = 74,
  KEY_PAGEUP = 75,
  KEY_DELETE = 76,
  KEY_END = 77,
  KEY_PAGEDOWN = 78,
  KEY_RIGHT = 79,
  KEY_LEFT = 80,
  KEY_DOWN = 81,
  KEY_UP = 82,

  KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                    */
  KEY_KP_DIVIDE = 84,
  KEY_KP_MULTIPLY = 85,
  KEY_KP_MINUS = 86,
  KEY_KP_PLUS = 87,
  KEY_KP_ENTER = 88,
  KEY_KP_1 = 89,
  KEY_KP_2 = 90,
  KEY_KP_3 = 91,
  KEY_KP_4 = 92,
  KEY_KP_5 = 93,
  KEY_KP_6 = 94,
  KEY_KP_7 = 95,
  KEY_KP_8 = 96,
  KEY_KP_9 = 97,
  KEY_KP_0 = 98,
  KEY_KP_PERIOD = 99,

  KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                      *   keyboards have over ANSI ones,
                                      *   located between left shift and Y.
                                      *   Produces GRAVE ACCENT and TILDE in a
                                      *   US or UK Mac layout, REVERSE SOLIDUS
                                      *   (backslash) and VERTICAL LINE in a
                                      *   US or UK Windows layout, and
                                      *   LESS-THAN SIGN and GREATER-THAN SIGN
                                      *   in a Swiss German, German, or French
                                      *   layout. */
  KEY_APPLICATION = 101, /**< windows contextual menu, compose */
  KEY_POWER = 102, /**< The USB document says this is a status flag,
                              *   not a physical key - but some Mac keyboards
                              *   do have a power key. */
  KEY_KP_EQUALS = 103,
  KEY_F13 = 104,
  KEY_F14 = 105,
  KEY_F15 = 106,
  KEY_F16 = 107,
  KEY_F17 = 108,
  KEY_F18 = 109,
  KEY_F19 = 110,
  KEY_F20 = 111,
  KEY_F21 = 112,
  KEY_F22 = 113,
  KEY_F23 = 114,
  KEY_F24 = 115,
  KEY_EXECUTE = 116,
  KEY_HELP = 117,
  KEY_MENU = 118,
  KEY_SELECT = 119,
  KEY_STOP = 120,
  KEY_AGAIN = 121,   /**< redo */
  KEY_UNDO = 122,
  KEY_CUT = 123,
  KEY_COPY = 124,
  KEY_PASTE = 125,
  KEY_FIND = 126,
  KEY_MUTE = 127,
  KEY_VOLUMEUP = 128,
  KEY_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     KEY_LOCKINGCAPSLOCK = 130,  */
/*     KEY_LOCKINGNUMLOCK = 131, */
/*     KEY_LOCKINGSCROLLLOCK = 132, */
  KEY_KP_COMMA = 133,
  KEY_KP_EQUALSAS400 = 134,

  KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                          footnotes in USB doc */
  KEY_INTERNATIONAL2 = 136,
  KEY_INTERNATIONAL3 = 137, /**< Yen */
  KEY_INTERNATIONAL4 = 138,
  KEY_INTERNATIONAL5 = 139,
  KEY_INTERNATIONAL6 = 140,
  KEY_INTERNATIONAL7 = 141,
  KEY_INTERNATIONAL8 = 142,
  KEY_INTERNATIONAL9 = 143,
  KEY_LANG1 = 144, /**< Hangul/English toggle */
  KEY_LANG2 = 145, /**< Hanja conversion */
  KEY_LANG3 = 146, /**< Katakana */
  KEY_LANG4 = 147, /**< Hiragana */
  KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
  KEY_LANG6 = 149, /**< reserved */
  KEY_LANG7 = 150, /**< reserved */
  KEY_LANG8 = 151, /**< reserved */
  KEY_LANG9 = 152, /**< reserved */

  KEY_ALTERASE = 153, /**< Erase-Eaze */
  KEY_SYSREQ = 154,
  KEY_CANCEL = 155,
  KEY_CLEAR = 156,
  KEY_PRIOR = 157,
  KEY_RETURN2 = 158,
  KEY_SEPARATOR = 159,
  KEY_OUT = 160,
  KEY_OPER = 161,
  KEY_CLEARAGAIN = 162,
  KEY_CRSEL = 163,
  KEY_EXSEL = 164,

  KEY_KP_00 = 176,
  KEY_KP_000 = 177,
  KEY_THOUSANDSSEPARATOR = 178,
  KEY_DECIMALSEPARATOR = 179,
  KEY_CURRENCYUNIT = 180,
  KEY_CURRENCYSUBUNIT = 181,
  KEY_KP_LEFTPAREN = 182,
  KEY_KP_RIGHTPAREN = 183,
  KEY_KP_LEFTBRACE = 184,
  KEY_KP_RIGHTBRACE = 185,
  KEY_KP_TAB = 186,
  KEY_KP_BACKSPACE = 187,
  KEY_KP_A = 188,
  KEY_KP_B = 189,
  KEY_KP_C = 190,
  KEY_KP_D = 191,
  KEY_KP_E = 192,
  KEY_KP_F = 193,
  KEY_KP_XOR = 194,
  KEY_KP_POWER = 195,
  KEY_KP_PERCENT = 196,
  KEY_KP_LESS = 197,
  KEY_KP_GREATER = 198,
  KEY_KP_AMPERSAND = 199,
  KEY_KP_DBLAMPERSAND = 200,
  KEY_KP_VERTICALBAR = 201,
  KEY_KP_DBLVERTICALBAR = 202,
  KEY_KP_COLON = 203,
  KEY_KP_HASH = 204,
  KEY_KP_SPACE = 205,
  KEY_KP_AT = 206,
  KEY_KP_EXCLAM = 207,
  KEY_KP_MEMSTORE = 208,
  KEY_KP_MEMRECALL = 209,
  KEY_KP_MEMCLEAR = 210,
  KEY_KP_MEMADD = 211,
  KEY_KP_MEMSUBTRACT = 212,
  KEY_KP_MEMMULTIPLY = 213,
  KEY_KP_MEMDIVIDE = 214,
  KEY_KP_PLUSMINUS = 215,
  KEY_KP_CLEAR = 216,
  KEY_KP_CLEARENTRY = 217,
  KEY_KP_BINARY = 218,
  KEY_KP_OCTAL = 219,
  KEY_KP_DECIMAL = 220,
  KEY_KP_HEXADECIMAL = 221,

  KEY_LCTRL = 224,
  KEY_LSHIFT = 225,
  KEY_LALT = 226, /**< alt, option */
  KEY_LGUI = 227, /**< windows, command (apple), meta */
  KEY_RCTRL = 228,
  KEY_RSHIFT = 229,
  KEY_RALT = 230, /**< alt gr, option */
  KEY_RGUI = 231, /**< windows, command (apple), meta */

  KEY_MODE = 257,    /**< I'm not sure if this is really not covered
                                *   by any of the above, but since there's a
                                *   special KMOD_MODE for it I'm adding it here
                                */

  /* @} *//* Usage page 0x07 */

  /**
    *  \name Usage page 0x0C
    *
    *  These values are mapped from usage page 0x0C (USB consumer page).
    */
  /* @{ */

  KEY_AUDIONEXT = 258,
  KEY_AUDIOPREV = 259,
  KEY_AUDIOSTOP = 260,
  KEY_AUDIOPLAY = 261,
  KEY_AUDIOMUTE = 262,
  KEY_MEDIASELECT = 263,
  KEY_WWW = 264,
  KEY_MAIL = 265,
  KEY_CALCULATOR = 266,
  KEY_COMPUTER = 267,
  KEY_AC_SEARCH = 268,
  KEY_AC_HOME = 269,
  KEY_AC_BACK = 270,
  KEY_AC_FORWARD = 271,
  KEY_AC_STOP = 272,
  KEY_AC_REFRESH = 273,
  KEY_AC_BOOKMARKS = 274,

  /* @} *//* Usage page 0x0C */

  /**
    *  \name Walther keys
    *
    *  These are values that Christian Walther added (for mac keyboard?).
    */
  /* @{ */

  KEY_BRIGHTNESSDOWN = 275,
  KEY_BRIGHTNESSUP = 276,
  KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                          switch, video mode switch */
  KEY_KBDILLUMTOGGLE = 278,
  KEY_KBDILLUMDOWN = 279,
  KEY_KBDILLUMUP = 280,
  KEY_EJECT = 281,
  KEY_SLEEP = 282,

  KEY_APP1 = 283,
  KEY_APP2 = 284,

  /* @} *//* Walther keys */

  /**
    *  \name Usage page 0x0C (additional media keys)
    *
    *  These values are mapped from usage page 0x0C (USB consumer page).
    */
  /* @{ */

  KEY_AUDIOREWIND = 285,
  KEY_AUDIOFASTFORWARD = 286,

  /* @} *//* Usage page 0x0C (additional media keys) */

  /* Add any other keys here. */

  KEY_COUNT = 512 /**< not a key, just marks the number of scancodes
                                for array bounds */
};
// }}}

enum { // MouseKey
  MOUSE_KEY_LEFT = 0,
  MOUSE_KEY_RIGHT,
  MOUSE_KEY_MIDDLE,

  MOUSE_KEY_COUNT,
};

typedef struct {
  f32 x, y;
  u8  previous[MOUSE_KEY_COUNT];
  u8  current [MOUSE_KEY_COUNT];
  f32 duration[MOUSE_KEY_COUNT];
}Mouse;

typedef struct {
  u8  previous[KEY_COUNT];
  u8  current [KEY_COUNT];
  f32 duration[KEY_COUNT];
}Keyboard;

typedef struct {
  u64 milliseconds;
  f64 delta_time; // the last increment between the last frame and currnet frame in milliseconds.
}Clock;

typedef void Chunk;
//typedef void Music;

// statically linked gles2 lib
PLT_DEF void plt_init_gles2_static(const char *window_name, i32 width, i32 height);
PLT_DEF void plt_quit             (void);
PLT_DEF b32  plt_poll_events      (void);

PLT_DEF void plt_gl_swap_buffers(void);
PLT_DEF void plt_gl_set_vsync   (b32 is_on);

PLT_DEF void plt_window_fullscreen(b32 is_fullscreen);
PLT_DEF void plt_window_size      (i32 *w, i32 *h);

PLT_DEF void plt_log_info (const char *fmt, ...);
PLT_DEF void plt_log_warn (const char *fmt, ...);
PLT_DEF void plt_log_error(const char *fmt, ...);

PLT_DEF void plt_bin_save_to_desk  (const char *filename, u8 *__restrict buffer, size_t buffer_size);
PLT_DEF u8 * plt_bin_read_from_desk(const char *filename, size_t *size);
PLT_DEF void plt_bin_free(void *buffer);

PLT_DEF const Mouse*    plt_mouse   (void);
PLT_DEF const Keyboard* plt_keyboard(void);
PLT_DEF const Clock*    plt_clock   (void);

PLT_DEF b32   plt_key_clicked (Key);
PLT_DEF b32   plt_key_released(Key);

PLT_DEF b32   plt_mouse_clicked (MouseKey);
PLT_DEF b32   plt_mouse_released(MouseKey);


PLT_DEF Chunk * plt_chunk_load(const u8 *data, size_t size);
PLT_DEF void    plt_chunk_free(Chunk *chunk);
// If the specified channel is -1, play on the first free channel (and return  -1 without playing anything new if no free channel was available).
// If `loops` is greater than zero, loop the sound that many times. If `loops` is -1, loop "infinitely" (~65000 times).
// returns:  which channel was used to play the sound, or -1 if sound could not be played.
PLT_DEF i32     plt_channel_play(i32 channel, Chunk *chunk, i32 loops);
// If the specified channel is -1, all channels are checked, and this function returns the number of channels currently playing.
PLT_DEF i32     plt_channel_is_playing(i32 channel);

//PLT_DEF i32 plt_music_play();
//PLT_DEF Music * plt_music_load(const u8 *data, size_t size);
//PLT_DEF void    plt_music_free(Music *music);

#define UNUSED(x) ((void)(x))

#ifdef DEBUG
# define AssertHelper(expr, file, line, function) do {                                   \
    if (!(expr)) {                                                                       \
      fprintf(stderr, "%s:%d: Assertion :: %s -> %s(..)\n", file, line, #expr, function);\
      abort();                                                                           \
    }                                                                                    \
  }while(0)
#else
# define AssertHelper(expr, file, line, function) UNUSED(expr); UNUSED(file); UNUSED(line); UNUSED(function);
#endif

#define Assert(expr) AssertHelper(expr, __FILE__, __LINE__, __func__)

#define UNREACHABLE_CASE default: Assert(0 && "Unreachable Case!")

// PLT_IMPLEMENTATION {{{
#ifdef PLT_IMPLEMENTATION

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static struct System {
  SDL_Window    *window;
  SDL_GLContext gl_context;

  Mouse    mouse;
  Keyboard keyboard;
  Clock    clock;
} *g_sys;

internal void clock_update (Clock *clock);
internal void mouse_tick   (Mouse *mouse);
internal void keyboard_tick(Keyboard *keyboard);

void
plt_init_gles2_static(const char *window_name, i32 width, i32 height)
{
  Assert(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == 0);

  g_sys = malloc(sizeof(*g_sys));
  memset(g_sys, 0, sizeof(*g_sys));

  ////////////////
  ///// init video
  ////////////////

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#ifdef DEBUG
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  u32 window_flags = SDL_WINDOW_OPENGL;
    //| SDL_WINDOW_RESIZABLE);

  g_sys->window = SDL_CreateWindow(window_name,
          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          width, height,
          window_flags);

  Assert(g_sys->window);

  g_sys->gl_context = SDL_GL_CreateContext(g_sys->window);
  Assert(g_sys->gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  SDL_ShowCursor(0); // hide cursor

  ////////////////
  ///// init audio
  ////////////////

#ifdef DEBUG
  i32 number_of_devices = SDL_GetNumAudioDevices(0);
  for ( i32 i = 0
      ; i < number_of_devices
      ; i ++ )
  {
    plt_log_info("audio device [%s]", SDL_GetAudioDeviceName(i, 0));
  }
#endif

  i32 audio_device_success;

  i32 frequency = 48000;
  u16 format    = AUDIO_S16SYS;
  i32 nchannels = 2;            // Stereo
  i32 chunksize = 1024;         // 2048, 4096

  audio_device_success = Mix_OpenAudioDevice(frequency, format, nchannels, chunksize, NULL,
      SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);

  Assert((audio_device_success == 0) && "failed to open audio device");
}

void
plt_quit(void)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  SDL_GL_DeleteContext(g_sys->gl_context);
  SDL_DestroyWindow(g_sys->window);

  Mix_CloseAudio();

  SDL_Quit();
  free(g_sys);
}

b32
plt_poll_events(void)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");

  clock_update(&g_sys->clock);
  mouse_tick(&g_sys->mouse);
  keyboard_tick(&g_sys->keyboard);

  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT: { return 0; }break;

      case SDL_KEYDOWN:
      {
        g_sys->keyboard.current[e.key.keysym.scancode] = 1;
      }break;

      case SDL_MOUSEMOTION:
      {
        g_sys->mouse.x = e.motion.x;
        g_sys->mouse.y = e.motion.y;
      }break;

      case SDL_MOUSEBUTTONDOWN:
      {
        switch (e.button.button)
        {
          case SDL_BUTTON_LEFT:   { g_sys->mouse.current[MOUSE_KEY_LEFT]   = 1; }break;
          case SDL_BUTTON_MIDDLE: { g_sys->mouse.current[MOUSE_KEY_MIDDLE] = 1; }break;
          case SDL_BUTTON_RIGHT:  { g_sys->mouse.current[MOUSE_KEY_RIGHT]  = 1; }break;
          case SDL_BUTTON_X1:     { }break;
          case SDL_BUTTON_X2:     { }break;

          UNREACHABLE_CASE;
        }
      }break;

      default: {} // do noting
    }
  }

  return 1;
}

void
plt_gl_swap_buffers(void)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  SDL_GL_SwapWindow(g_sys->window);
}

void
plt_gl_set_vsync(b32 is_on)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  SDL_GL_SetSwapInterval(is_on);
}

void
plt_window_fullscreen(b32 is_fullscreen)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  SDL_SetWindowFullscreen(g_sys->window, SDL_WINDOW_FULLSCREEN_DESKTOP & is_fullscreen);
}

void
plt_window_size(i32 *w, i32 *h)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  SDL_GetWindowSize(g_sys->window, w, h);
}

#define LOG_CAPACITY     2048L

void
plt_log_info (const char *fmt, ...)
{
  char buffer[LOG_CAPACITY];

  va_list va;
  va_start(va, fmt);
  vsnprintf(buffer, LOG_CAPACITY, fmt, va);
  va_end(va);

  fprintf(stdout, "info: %s\n", buffer);
}

void
plt_log_warn (const char *fmt, ...)
{
  char buffer[LOG_CAPACITY];

  va_list va;
  va_start(va, fmt);
  vsnprintf(buffer, LOG_CAPACITY, fmt, va);
  va_end(va);

  fprintf(stderr, "warn: %s\n", buffer);
}

void
plt_log_error(const char *fmt, ...)
{
  char buffer[LOG_CAPACITY];

  va_list va;
  va_start(va, fmt);
  vsnprintf(buffer, LOG_CAPACITY, fmt, va);
  va_end(va);

  fprintf(stderr, "error: %s\n", buffer);
}

void
plt_bin_save_to_desk(const char *filename, u8 *__restrict buffer, size_t buffer_size)
{
  FILE *file = fopen(filename, "wb");
  size_t fsize = fwrite(buffer, 1, buffer_size, file);
  Assert(fsize == buffer_size);
  fclose(file);
}

u8 *
plt_bin_read_from_desk(const char *filename, size_t *size)
{
  u8 *buffer;

  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    plt_log_error("failed to open %s", filename);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  size_t buffer_size = ftell(file);
  rewind(file);
  if (buffer_size == 0)
  {
    plt_log_error("failed to open [filesize = 0] %s", filename);
    return NULL;
  }

  buffer = malloc(buffer_size);
  Assert(buffer);

  size_t read_size = fread(buffer, 1, buffer_size, file);
  Assert(read_size == buffer_size);

  fclose(file);
  if (size)
    *size = buffer_size;

  return buffer;
}

void
plt_bin_free(void *buffer)
{
  free(buffer);
}

const Mouse*
plt_mouse(void)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  return &g_sys->mouse;
}

const Keyboard*
plt_keyboard(void)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  return &g_sys->keyboard;
}

const Clock*
plt_clock(void)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  return &g_sys->clock;
}

b32
plt_key_clicked(Key key)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  Assert(key < KEY_COUNT);

  return g_sys->keyboard.current[key] && !g_sys->keyboard.previous[key];
}

b32
plt_key_released(Key key)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  Assert(key < KEY_COUNT);

  return !g_sys->keyboard.current[key] && g_sys->keyboard.previous[key];
}

b32
plt_mouse_clicked(MouseKey key)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  Assert(key < MOUSE_KEY_COUNT);

  return g_sys->mouse.current[key] && !g_sys->mouse.previous[key];
}

b32
plt_mouse_released(MouseKey key)
{
  Assert(g_sys && "Forgot to init -> use plt_init_*() first.");
  Assert(key < MOUSE_KEY_COUNT);

  return !g_sys->mouse.current[key] && g_sys->mouse.previous[key];
}

internal void
clock_update(Clock *clock)
{
  u64 new_tick = SDL_GetTicks64();
  clock->delta_time = (new_tick - clock->milliseconds);
  clock->milliseconds = new_tick;
}

internal void
mouse_tick(Mouse *mouse)
{
  if (mouse->previous[MOUSE_KEY_LEFT]   && mouse->current[MOUSE_KEY_LEFT])
  {
    mouse->duration[MOUSE_KEY_LEFT]   += g_sys->clock.delta_time;
  }
  if (mouse->previous[MOUSE_KEY_MIDDLE] && mouse->current[MOUSE_KEY_MIDDLE])
  {
    mouse->duration[MOUSE_KEY_MIDDLE] += g_sys->clock.delta_time;
  }
  if (mouse->previous[MOUSE_KEY_RIGHT]  && mouse->current[MOUSE_KEY_RIGHT])
  {
    mouse->duration[MOUSE_KEY_RIGHT]  += g_sys->clock.delta_time;
  }

  mouse->previous[MOUSE_KEY_LEFT]   = mouse->current[MOUSE_KEY_LEFT];
  mouse->previous[MOUSE_KEY_MIDDLE] = mouse->current[MOUSE_KEY_MIDDLE];
  mouse->previous[MOUSE_KEY_RIGHT]  = mouse->current[MOUSE_KEY_RIGHT];

  mouse->current[MOUSE_KEY_LEFT]   = 0;
  mouse->current[MOUSE_KEY_MIDDLE] = 0;
  mouse->current[MOUSE_KEY_RIGHT]  = 0;
}

internal void
keyboard_tick(Keyboard *keyboard)
{
  for ( size_t i = 0
      ; i < SDL_NUM_SCANCODES
      ; i ++ )
  {
    if (keyboard->previous[i] && keyboard->current[i])
    {
      keyboard->duration[i] += g_sys->clock.delta_time;
    }

    keyboard->previous[i] = keyboard->current[i];
    keyboard->current[i] = 0;
  }
}

PLT_DEF Chunk *
plt_chunk_load(const u8 *data, size_t size)
{
  return (Chunk*)Mix_QuickLoad_RAW((u8*)data, size);
}

PLT_DEF void
plt_chunk_free(Chunk *chunk)
{
  Mix_FreeChunk((Mix_Chunk*)chunk);
}

PLT_DEF i32
plt_channel_play(i32 channel, Chunk *chunk, i32 loops)
{
  return Mix_PlayChannel(channel, (Mix_Chunk*)chunk, loops);
}

// If the specified channel is -1, all channels are checked, and this function returns the number of channels currently playing.
PLT_DEF i32
plt_channel_is_playing(i32 channel)
{
  return Mix_Playing(channel);
}

#endif // PLT_IMPLEMENTATION
// }}} PLT_IMPLEMENTATION

// vim: sw=2 fdm=marker
