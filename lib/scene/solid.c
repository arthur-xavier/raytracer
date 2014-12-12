#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "solid.h"
#include "ray.h"

bool solid_intersection(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection)
{
  intersection->solid = NULL;
  intersection->ray = ray;
  return solid->function(solid, ray, intersection);
}

void solid_translate(SOLID* solid, const float* t) {
  size_t i;
  for(i = 0; i < solid->num_points; i++) {
    v_add(&solid->points[i*3], t, &solid->points[i*3]);
  }
}
void solid_scale(SOLID* solid, const float* s) {
  size_t i;
  for(i = 0; i < solid->num_points; i++) {
    v_mulv(&solid->points[i*3], s, &solid->points[i*3]);
  }
}
void solid_rotate(SOLID* solid, const float* q) {
  // TODO
}


bool SphereFunction(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection)
{
  float dist[3];
  const VECTOR centre = solid->points;
  float radius = solid->points[3];
  v_sub(ray->origin, centre, dist);
  float a = v_dot(ray->direction, ray->direction);
  float b = 2*v_dot(ray->direction, dist);
  float c = v_dot(dist, dist) - pow(radius, 2);
  float delta = b*b - 4*a*c;

  float north[] = { 0.0f, 1.0f, 0.0f };
  float equator[] = { 0.0f, 0.0f, 1.0f };

  float t1, t2;

  if(delta >= 0) {
    intersection->solid = solid;
    t1 = (-b + sqrtf(delta))/(2*a);
    t2 = (-b - sqrtf(delta))/(2*a);
    intersection->t_in = fmin(t1, t2);
    intersection->t_out = fmax(t1, t2);

    // intersection->point = t*direction + origin;
    v_mul(intersection->t_in, ray->direction, intersection->point);
    v_add(ray->origin, intersection->point, intersection->point);

    v_sub(intersection->point, centre, dist);
    v_normalize(dist, intersection->normal);

    float phi = acosf(-v_dot(north, intersection->normal));
    intersection->texture[1] = phi / M_PI;

    float theta = acosf(v_dot(equator, intersection->normal) / sin(phi)) / (2*M_PI);
    v_cross(north, equator, equator);
    if(v_dot(equator, intersection->normal) > 0)
      intersection->texture[0] = theta;
    else
      intersection->texture[0] = 1 - theta;

    v_clamp(intersection->texture, 0.0f, 1.0f, intersection->texture);
  }

  return (intersection->solid != NULL);
}

bool PlaneFunction(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection) {
  /**
  Ray: p = o + t*d
  Plane: (p - p0)·n = 0

  (o + t*d - p0)·n = 0
  (o + t*d)·n - p0·n = 0
  t*(d·n) + (o - p0)·n = 0
  t = (p0 - o)·n ÷ (d·n)
  */
  float a[3];
  float* normal = &solid->points[3];
  v_normalize(normal, normal);

  intersection->t_in = ray->far;
  intersection->t_out = ray->far;

  if(v_dot(ray->direction, normal) != 0.0f) {
    v_sub(&solid->points[0], ray->origin, a);
    //v_sub(ray->origin, &solid->points[0], a);
    intersection->t_in = v_dot(a, normal) / v_dot(ray->direction, normal);
    //fprintf(stderr, "Hit plane at t = %.3f\n", intersection->t_in);
    intersection->t_out = intersection->t_in;

    intersection->solid = solid;

    v_copy(intersection->normal, normal);
    // intersection->point = t*direction + origin;
    v_mul(intersection->t_in, ray->direction, intersection->point);
    v_add(ray->origin, intersection->point, intersection->point);

    return true;
  }
  return false;
}

bool TriangleFunction(SOLID* solid, RAY* ray, RAY_INTERSECTION* intersection)
{
  size_t i;
  float *a, *b, *c;

  float ab[3], ac[3];
  float ao[3], p[3], q[3];
  float t, u, v;
  float det;

  intersection->t_in = ray->far;
  intersection->t_out = ray->near;

  for(i = 0; i < solid->indices[0]*3;) {
    a = &solid->points[solid->indices[++i]*3];
    b = &solid->points[solid->indices[++i]*3];
    c = &solid->points[solid->indices[++i]*3];

    /* MÖLLER-TRUMBORE RAY-TRIANGLE INTERSECTION ALGORITHM */
    v_sub(b, a, ab);
    v_sub(c, a, ac);

    v_cross(ray->direction, ac, p);
    det = v_dot(ab, p);
    if(det <= 0.0f)
      continue;
    v_sub(ray->origin, a, ao);
    u = v_dot(ao, p) / det;
    if(u < 0.0f || u > 1.0f)
      continue;
    v_cross(ao, ab, q);
    v = v_dot(ray->direction, q) / det;
    if(v < 0.0f || u + v > 1.0f)
      continue;

    t = v_dot(ac, q) / det;
    // if ray intersects triangle
    if(t > 0.0f) {
      intersection->solid = solid;

      if(t < intersection->t_in) {
        intersection->t_in = t;
        // set texCoords
        v_set(intersection->texture, u, v, 0.0f);
        // set normal
        v_cross(ab, ac, intersection->normal);
        v_normalize(intersection->normal, intersection->normal);
        // intersection->point = t*direction + origin;
        v_mul(intersection->t_in, ray->direction, intersection->point);
        v_add(ray->origin, intersection->point, intersection->point);
      }

      if(t > intersection->t_out)
        intersection->t_out = t;
    }
  }

  return (intersection->solid != NULL);
}
