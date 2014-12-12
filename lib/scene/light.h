/**
 * Defines a simple light structure
 */
#ifndef LIGHT_H_
#define LIGHT_H_

/**
 * Simple light structure that defines mainly
 * its spatial and shading parameters.
 */
typedef struct {
  enum {
    POINT,
    DIRECTIONAL
  } type;
  float position[3];
  float color[3];
  float intensity;
} LIGHT;

#endif
