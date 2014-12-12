#include "vector.h"

float vector_distance(const VECTOR u, const VECTOR v) {
  float d[3];
  vector_sub(v, u, d);
  return vector_length(d);
}

float* vector_proj(const VECTOR u, const VECTOR v, VECTOR r) {
  float s = vector_dot(u, v) / vector_length(u);
  vector_mul(s, v, r);
  return r;
}
