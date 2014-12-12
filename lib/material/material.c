#include <stdio.h>
#include <math.h>
#include "material.h"
#include "ray.h"
#include "light.h"
#include "vector.h"
#include "image.h"

float* Lambert(MATERIAL* material, RAY_INTERSECTION* intersection, LIGHT* light, float* color) {
  float dist[3]; // distance to light
  float diffuse; // diffuse component
  // material parameters
  const float* diffuse_color = material->parameters;
  float kd = material->parameters[3];
  float sq;

  if(light->type == DIRECTIONAL) {
    v_mul(-1, light->position, dist);
    sq = 10.0f;
  } else {
    v_sub(light->position, intersection->point, dist);
    sq = v_dot(dist, dist);
  }
  v_normalize(dist, dist);
  diffuse = v_dot_(intersection->normal, dist)/sq * light->intensity * kd;

  v_mulv(diffuse_color, light->color, color);
  v_mul(diffuse, color, color);

  if(material->texture != NULL && material->texture->data != NULL) {
    float texture_color[3];
    image_getpixelf(material->texture,
      material->texture->width*intersection->texture[0], 
      material->texture->height*intersection->texture[1],
      texture_color);
    //v_mul(1.0f/255, texture_color, texture_color);
    v_clamp(texture_color, 0.0f, 1.0f, texture_color);
    //v_mul(1.0f/4.0f, texture_color, texture_color);
    v_mulv(color, texture_color, color);
    //v_copy(color, texture_color);
  }

  return color;
}

float* Phong(MATERIAL* material, RAY_INTERSECTION* intersection, LIGHT* light, float* color) {
  float dist[3];       // distance to light
  float view[3];       // distance to eye
  float reflection[3]; // reflected ray
  float diffuse;       // diffuse component
  float specular;      // specular component
  // material parameters
  float* specular_color = &material->parameters[4];
  float ks = material->parameters[7]; // specular coeficient
  float sq;

  Lambert(material, intersection, light, color);

  if(light->type == DIRECTIONAL) {
    v_mul(-1, light->position, dist);
    sq = 10.0f;
  } else {
    v_sub(light->position, intersection->point, dist);
    sq = v_dot(dist, dist);
  }
  v_normalize(dist, dist);
  diffuse = v_dot_(intersection->normal, dist);

  v_mul(-1, intersection->ray->direction, view);

  // reflection = normalize(2*diffuse*normal - dist)
  v_mul(2*diffuse, intersection->normal, reflection);
  v_sub(reflection, dist, reflection);
  v_normalize(reflection, reflection);

  specular = (ks > 0) ? pow(v_dot_(reflection, view), ks) : 0;

  v_mul(specular/sq * light->intensity * sqrtf(ks), specular_color, dist);
  v_add(color, dist, color);
  return color;
}
