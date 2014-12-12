/**!
 * Defines a basic Ray structure and ray related helpers.
 */
#ifndef RAY_H_
#define RAY_H_

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>

#define RAY_MAX_ITERATION 3

struct SCENE;

/**
 * Simple structure for a ray.
 */
typedef struct {
  float near;         /**< Minimum distance for a ray to hit something */
  float far;          /**< Maximum distance for a ray to hit something */

  float* origin;
  float direction[3];

  enum {
    CAST,
    ABSORBED,
    REFLECTED,
    REFRACTED
  } state;

  float length;
  u_short iteration;
} RAY;

/**
 * Represents an intersection of a ray and some solid object.
 */
typedef struct {
  RAY* ray;
  float t_in;          /**< Position on the ray where it enters a solid */
  float t_out;         /**< Position on the ray where it comes out of a solid */

  float texture[3];

  float point[3];      /**< Nearest intersection point */
  float normal[3];     /**< Normal at the intersection point */
  struct SOLID* solid; /**< Solid hit by the ray */
} RAY_INTERSECTION;

/**
 * Calculates a ray for the given origin and target points.
 * @param ray    Ray pointer
 * @param origin Origin point
 * @param target Target point
 */
void ray_calculate(RAY* ray, float* origin, float* target, bool direction);

/**
 * Tests for the intersection of the given ray with an array of solids.
 * @param ray          Ray
 * @param solids       Array of solids
 * @param n            Size of the solid array
 * @param intersection Nearest intersection of the ray with any solid from the array
 * @return There was an intersection with any solid
 */
bool ray_cast(RAY* ray, struct SOLID* solids, size_t n, RAY_INTERSECTION* intersection);

/**
 * Completely raytraces an entire scene.
 * @param ray   Ray
 * @param scene Scene
 * @param color Result color
 */
extern void ray_trace(RAY* ray, struct SCENE* scene, float* color);

#endif
