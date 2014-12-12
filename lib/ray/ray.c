#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "scene.h"
#include "ray.h"

void ray_calculate(RAY* ray, VECTOR origin, VECTOR target, bool direction) {
  ray->origin = origin;
  ray->iteration = 0;
  if(direction) {
    v_mul(-1, target, ray->direction);
  } else {
    v_sub(target, origin, ray->direction);
    v_normalize(ray->direction, ray->direction);
  }
}

bool ray_cast(RAY* ray, SOLID* solids, size_t n, RAY_INTERSECTION* intersection) {
  SOLID* s;
  RAY_INTERSECTION i;
  float nearest = ray->far;

  ray->state = CAST;

  ray->length = ray->far;
  intersection->solid = NULL;

  for(s = solids; s < solids + n; s++) {
    if(solid_intersection(s, ray, &i) && i.t_in < nearest && (i.t_in > ray->near || i.t_out > ray->near)) {
      *intersection = i;
      nearest = i.t_in;
      ray->length = v_distance(i.point, ray->origin);
    }
  }
  return (intersection != NULL && intersection->solid != NULL);
}

void ray_trace(RAY* ray, SCENE* scene, float* color) {
  float distance;
  float dist[3];
  float incidence[3];
  float reflection[3];
  float temp_color[3];

  RAY ray2;
  RAY_INTERSECTION i, ii;

  ray2.near = ray->near;
  ray2.far  = ray->far;

  LIGHT* l;

  // cast ray to the solids
  if(ray->iteration++ < RAY_MAX_ITERATION && ray_cast(ray, scene->solids, scene->n_solids, &i)) {
    // has ambient color
    v_copy(color, scene->ambient_color);
    // cast rays towards all the lights to check for shadows
    for(l = scene->lights; l < scene->lights + scene->n_lights; l++) {
      // cast ray towards the ligh
      ray_calculate(&ray2, i.point, l->position, (l->type == DIRECTIONAL));
      if(l->type == DIRECTIONAL) {
        v_mul(-1, l->position, dist);
        distance = ray->far;
      } else {
        v_sub(l->position, i.point, dist);
        distance = v_distance(i.point, l->position);
      }

      if(v_dot(i.normal, dist) <= 0.0f)
        continue;

      // if the point is not occluded by any solid for the light l,
      // then it got no shadow
      if(!(ray_cast(&ray2, scene->solids, scene->n_solids, &ii) && ii.t_in < distance)) {
        i.solid->material.function(&i.solid->material, &i, l, temp_color);
        v_add(color, temp_color, color);
      }
    }

    if(i.solid->material.reflectance > 0.0f && i.solid->material.reflectance <= 1.0f) {
      // reflection = 2(normal·incidence)*normal - incidence
      v_sub(i.point, ray->origin, incidence);
      v_normalize(incidence, incidence);
      v_mul(2*v_dot(i.normal, incidence), i.normal, reflection);
      v_sub(incidence, reflection, reflection);

      ray2.origin = i.point;
      ray2.iteration = ray->iteration;
      v_copy(ray2.direction, reflection);

      ray_trace(&ray2, scene, temp_color);
      v_mul(i.solid->material.reflectance, temp_color, temp_color);
      v_add(color, temp_color, color);
      //v_mulv(color, temp_color, color);
    }
  } else {
    v_copy(color, scene->background_color);
  }

  float L[3];
  const float B[3] = { 0.5f, 0.4f, 0.33f };
  const float Li[3] = { 0.13f, 0.15f, 0.22f };
  // Attenuation
  // E(d, λ) = I0(λ) · exp(-B(λ)d)/d²
  L[0] = expf(-B[0]*ray->length)/powf(ray->length, 2);
  L[1] = expf(-B[1]*ray->length)/powf(ray->length, 2);
  L[2] = expf(-B[2]*ray->length)/powf(ray->length, 2);
  v_clamp(L, 0.0f, 1.0f, L);
  //v_mulv(L, color, color);

  // Airlight
  // L(d, λ) = L∞(λ)(1 − exp(−β(λ)d)) 
  L[0] = Li[0]*(1 - exp(-B[0]*ray->length));
  L[1] = Li[1]*(1 - exp(-B[1]*ray->length));
  L[2] = Li[2]*(1 - exp(-B[2]*ray->length));
  //v_add(color, L, color);
}
