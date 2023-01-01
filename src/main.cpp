#include "platform.h"
#include "renderer.h"
#include "vmath.h"

internal void do_logic(void);
internal void do_drawing(void);

global Renderer * G_RENDERER;

int main(void) {

  plt_init_gles2_static("Space Invader", 1600, 900);
  G_RENDERER = rend_init();

  while (plt_poll_events()) {
    rend_tick(G_RENDERER);
    if (plt_keyboard()->current[KEY_ESCAPE]) {
      plt_log_info("Quiting Gracefully");
      goto QUIT;
    }

    do_logic();

    do_drawing();
  }

QUIT:
  rend_drop(G_RENDERER);
  plt_quit();
  return 0;
}

internal void
do_logic (void) {

}

internal void
do_drawing (void) {
  quad q;
  q.min.x = 100;
  q.min.y = 100;
  q.max.x = 300;
  q.max.y = 200;

  vec2 cursor;
  cursor.x = plt_mouse()->x;
  cursor.y = plt_mouse()->y;

  if (quad_collision_point(q, cursor)) {
    rend_draw_quad_filled(G_RENDERER, q, 0xFF00FFFF);
  } else {
    rend_draw_quad_filled(G_RENDERER, q, 0x00FFFFFF);
  }

  rend_draw_text(G_RENDERER, q, 0xFFFFFFFF, "this is a text to be rendered");
  q.max.x -= 20;
  rend_draw_text(G_RENDERER, q, 0xFF0000FF, "this is a text to be rendered");
  q.max.x -= 20;
  rend_draw_text(G_RENDERER, q, 0x0000FFFF, "this is a text to be rendered");

}
