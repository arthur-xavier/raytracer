#ifndef SCENE_H_
#define SCENE_H_

#include "solid.h"
#include "light.h"

typedef struct SCENE {
  size_t n_solids;
  SOLID* solids;

  size_t n_lights;
  LIGHT* lights;

  float ambient_color[3];
  float background_color[3];
} SCENE;

#endif
