#pragma once

#include "platform.h"
#include "renderer.h"
#include "vmath.h"

internal inline b32
has_flag(u32 k, u32 f)
{
  return (k & f) != 0;
}
