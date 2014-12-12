/**
 * Defines a generic Solid structure and Intersection Functions for
 * some solid types.
 */
#ifndef SOLID_H_
#define SOLID_H_

#include "vector.h"
#include "ray.h"
#include "light.h"
#include "material.h"

/**
 * Defines a generic solid composed by an array of points and indices,
 * a material, and a ray intersection test function.
 */
typedef struct SOLID {
  size_t num_points; /**< number of points */
  float* points;     /**< point array */
  float* texCoords;  /**< texture u,v coordinates array */
  size_t* indices;   /**< index array */
  MATERIAL material; /**< solid material */

  bool(*function)(struct SOLID*, RAY*, RAY_INTERSECTION*); /**< ray test function */
} SOLID;

/**
 * Calls the ray intersection function of a solid.
 * @param solid        Solid
 * @param ray          Ray
 * @param intersection Resulting intersection data
 * @return The ray has intersected the solid
 */
bool solid_intersection(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection);

void solid_translate(SOLID* solid, const float* t);
void solid_scale(SOLID* solid, const float* s);
void solid_rotate(SOLID* solid, const float* q);


#define SPHERE SphereFunction
/**
 * Solves a quadratic equation to test for the intersection between
 * a ray and a sphere.
 * The point array of a sphere is of the form: {[center], radius}
 * @param solid        Sphere
 * @param ray          Ray
 * @param intersection Resulting intersection data
 */
bool SphereFunction(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection);

#define PLANE PlaneFunction
bool PlaneFunction(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection);


#define TRIANGLE TriangleFunction
/**
 * Uses the Möller-Trumbore algorithm to test for the intersection
 * between a ray and a triangle
 * The point array of a triangle solid is of the form: {{[vertices]}},
 * and the index array gives the information about the connection
 * of the vertices.
 * @param solid        Sphere
 * @param ray          Ray
 * @param intersection Resulting intersection data
 */
bool TriangleFunction(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection);

#endif
