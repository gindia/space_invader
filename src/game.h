#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef u32 EntityFlags;
typedef u32 AiState;

enum { // EntityFlags
  ENTITY_FLAG_ACTIVE = 1 << 0,

  ENTITY_FLAG_BULLET = 1 << 1,
  ENTITY_FLAG_ENEMY  = 1 << 2,

  ENTITY_FLAG_COUNT,
};


typedef struct {
  EntityFlags flags;
  Animation   animation;
  quad        bounding_box;
}Entity;

typedef struct {
  Entity player;
  Entity player_exhaust;

  f32 spwan_timer;

#define GAME_MAX_ENTITIES 512
  Entity entity[GAME_MAX_ENTITIES];
}Game;

void game_init(Game *, const u8 *level, size_t level_len);
void game_update_and_render(Game *, Renderer *, f32 delta_time);

#ifdef __cplusplus
} // extern "C"
#endif
