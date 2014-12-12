/**!
 * Defines a simple Material structure and some material shading functions.
 */
#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "light.h"
#include "ray.h"

struct IMAGE;

/**
 * Simple Material structure that contains the material parameters
 * and a material shading function.
 */
typedef struct MATERIAL {
  float reflectance;
  float* parameters;
  struct IMAGE* texture;
  float*(*function)(struct MATERIAL*, RAY_INTERSECTION*, LIGHT*, float*); /**< shading function */
} MATERIAL;

#define LAMBERT Lambert
/**
 * Lambert shading model.
 * Its parameters are: {[diffuse color], diffuse coefficient}
 * @param parameters   Material parameters
 * @param intersection Ray intersection data
 * @param light        Light currently tested
 * @param color        Resulting color
 */
float* Lambert(MATERIAL* material, RAY_INTERSECTION* intersection, LIGHT* light, float* color);

#define PHONG Phong
/**
 * Phong shading model.
 * Its parameters are: {[diffuse color], diffuse coefficient,
 [specular color], specular coefficient}
 * @param parameters   Material parameters
 * @param intersection Ray intersection data
 * @param light        Light currently tested
 * @param color        Resulting color
 */
float* Phong(MATERIAL* material, RAY_INTERSECTION* intersection, LIGHT* light, float* color);

#endif
