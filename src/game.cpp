#include "game.h"

void
game_init(Game *game, const u8 *level, size_t level_len)
{
  Assert(game);
  UNUSED(level); UNUSED(level_len);
}

internal void
game__player_fire_bullet(Game *game, vec2 offset)
{
  size_t index;
  size_t i;

  Assert(game);

  for ( i = 0
      ; i < GAME_MAX_ENTITIES
      ; i ++ )
  {
    if (!((game->entity[i].flags & ENTITY_FLAG_ACTIVE) != 0)) {
      index = i;
      break;
    }
  }

  plt_log_info("new index = %lu", index);

  Assert(i < GAME_MAX_ENTITIES - 1);

  game->entity[index].flags  = ENTITY_FLAG_BULLET;
  game->entity[index].flags |= ENTITY_FLAG_ACTIVE;
  game->entity[index].animation.atlas_index = 0; // NOTE: assume 1st one is loaded

  game->entity[index].animation.sprite_index  [0] = 5;
  game->entity[index].animation.sprite_index  [1] = 6;
  game->entity[index].animation.sprite_index  [2] = 7;
  game->entity[index].animation.sprite_index  [3] = 8;
  game->entity[index].animation.duration_in_ms[0] = 100.0;
  game->entity[index].animation.duration_in_ms[1] = 100.0;
  game->entity[index].animation.duration_in_ms[2] = 100.0;
  game->entity[index].animation.duration_in_ms[3] = 100.0;
  game->entity[index].animation.animation_len = 4;
  game->entity[index].animation.current_frame = 0;
  game->entity[index].animation.taint = 0xFFFFFFFF;

  game->entity[index].bounding_box = quad_from_size(vec2_add(game->player.bounding_box.min, offset), vec2 {32.0, 32.0});
}

void
game_update_and_render(Game *game, Renderer *renderer, f32 delta_time)
{
  const Keyboard *kbd;
  const Mouse *mouse;
  vec2 cursor;

  kbd = plt_keyboard();
  mouse = plt_mouse();

  cursor.x = mouse->x;
  cursor.y = mouse->y;

  {
    game->player.bounding_box = quad_around_center(cursor, vec2 {32.0, 32.0});
    game->player_exhaust.bounding_box = quad_move(game->player.bounding_box, vec2{0.0, 32.0});

    if (plt_mouse_clicked(MOUSE_KEY_LEFT)) {
      game__player_fire_bullet(game, vec2 {0.0, 20.0});
      game__player_fire_bullet(game, vec2 {0.0, -20.0});
      game__player_fire_bullet(game, vec2 {0.0, 10.0});
      game__player_fire_bullet(game, vec2 {0.0, -10.0});
      game__player_fire_bullet(game, vec2 {0.0, 5.0});
    }

    for ( size_t i = 0
        ; i < GAME_MAX_ENTITIES
        ; i ++ )
    {
      Entity *entity = &game->entity[i];
      if ((entity->flags & (ENTITY_FLAG_BULLET | ENTITY_FLAG_ACTIVE)) != 0) {
        if (entity->bounding_box.min.y <= 0) {
          entity->flags &= ~ENTITY_FLAG_ACTIVE;
        } else {
          entity->bounding_box = quad_move(entity->bounding_box, vec2 {0.0, -10.0});
          rend_animaion_tick(&entity->animation, delta_time);
        }
      }
    }

    rend_animaion_tick(&game->player.animation, delta_time);
    rend_animaion_tick(&game->player_exhaust.animation, delta_time);
  }

  /// render

  {
    for ( size_t i = 0
        ; i < GAME_MAX_ENTITIES
        ; i ++ )
    {
      Entity *entity = &game->entity[i];
      if ((entity->flags & ENTITY_FLAG_ACTIVE) != 0) {
        rend_animation_draw(renderer, &entity->animation, entity->bounding_box);
      }
    }

    rend_animation_draw(renderer, &game->player.animation, game->player.bounding_box);
    rend_animation_draw(renderer, &game->player_exhaust.animation, game->player_exhaust.bounding_box);
  }
}
