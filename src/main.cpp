#include "platform.h"
#include "renderer.h"
#include "vmath.h"


internal void do_logic(void);
internal void do_drawing(void);

int main(void) {
  plt_init_gles2_static("Space Invader", 1600, 900);

  while (plt_poll_events()) {
    if (plt_keyboard()->current[KEY_ESCAPE]) {
      plt_log_info("Quiting Gracefully");
      goto QUIT;
    }

    do_logic();

    do_drawing();

    plt_gl_swap_buffers();
  }

QUIT:
  plt_quit();
  return 0;
}

internal void
do_logic (void) {

}

internal void
do_drawing (void) {
  //glClearColor(0.3, 0.3, 0.3, 1.0);
  //glClear(GL_COLOR_BUFFER_BIT);

}
