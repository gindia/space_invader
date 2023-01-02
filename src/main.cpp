#include "common.h"
#include "game.h"

int
main(void)
{
  Renderer *renderer;
  Game *game;
  AtlasIndex atlas_index;
  u8 *raw_image_buffer;
  size_t buffer_size;
  f32 delta_time;

  plt_init_gles2_static("Space Invader", 1600, 900);
  renderer = rend_init();

  raw_image_buffer = plt_bin_read_from_desk("res/space_invader.png", &buffer_size);
  Assert(raw_image_buffer);
  atlas_index = rend_atlas_load(renderer, raw_image_buffer, buffer_size, 1, 9, 32);
  //plt_log_info("atlas_index = %lu", atlas_index);

  game = (Game*)malloc(sizeof(*game));
  memset(game, 0, sizeof(*game));
  game_init(game, NULL, 0);

  game->player.animation.atlas_index = atlas_index;
  game->player.animation.sprite_index  [0] = 0;
  game->player.animation.duration_in_ms[0] = 100.0;
  game->player.animation.animation_len = 1;
  game->player.animation.current_frame = 0;
  game->player.animation.taint = 0xFFFFFFFF;
  game->player.bounding_box = quad_from_size(vec2{100.0, 100.0}, vec2{32.0, 32.0});

  game->player_exhaust.animation.atlas_index = atlas_index;
  game->player_exhaust.animation.sprite_index  [0] = 1;
  game->player_exhaust.animation.sprite_index  [1] = 2;
  game->player_exhaust.animation.sprite_index  [2] = 3;
  game->player_exhaust.animation.sprite_index  [3] = 4;
  game->player_exhaust.animation.duration_in_ms[0] = 100.0;
  game->player_exhaust.animation.duration_in_ms[1] = 100.0;
  game->player_exhaust.animation.duration_in_ms[2] = 100.0;
  game->player_exhaust.animation.duration_in_ms[3] = 100.0;
  game->player_exhaust.animation.animation_len = 4;
  game->player_exhaust.animation.current_frame = 0;
  game->player_exhaust.animation.taint = 0xFFFFFFFF;
  //game->player_exhaust.bounding_box = quad_from_size(vec2{100.0, 100.0}, vec2{32.0, 32.0});

  while (plt_poll_events()) {
    rend_tick(renderer);
    if (plt_key_clicked(KEY_ESCAPE)) {
      plt_log_info("Quiting Gracefully");
      goto QUIT;
    }

    delta_time = (f32)plt_clock()->delta_time;

    game_update_and_render(game, renderer, delta_time);
  }

QUIT:
  rend_drop(renderer);
  plt_quit();
  return 0;
}
