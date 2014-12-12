/**
 * Defines basic 3D Vector algebra and manipulation macros and functions.
 * All of the vector algebra functions and macros take as last argument
 * the vector where to put the result of the operation.
 */
#ifndef VECTOR_H_
#define VECTOR_H_

#include <memory.h>
#include <math.h>

#define VECTOR float*

// Aliases for vector functions and macros using the v_ prefix.
#define v_copy vector_copy
#define v_set vector_set
#define v_length vector_length
#define v_clamp vector_clamp
#define v_normalize vector_normalize
#define v_add vector_add
#define v_sub vector_sub
#define v_mul vector_mul
#define v_mulv vector_mulv
#define v_distance vector_distance
#define v_dot vector_dot
#define v_dot_ vector_dot_
#define v_cross vector_cross
#define v_proj vector_proj
#define v_function vector_function
#define v_function2 vector_function2

/**
 * Copies the value of a vector to another vector
 * @param u Result vector
 * @param v Vector to be copied
 */
#define vector_copy(u, v) \
  memcpy(u, v, sizeof(float)*3);

/**
 * Sets a vector components.
 * @param v     Vector
 * @param x,y,z Coordinates
 */
#define vector_set(v, x, y, z) \
  v[0] = x; \
  v[1] = y; \
  v[2] = z

/**
 * Adds two vectors and takes the result into another vector.
 * @param u,v Input vectors
 * @param r   Result vector
 */
#define vector_add(u, v, r) \
  vector_set((r), (u)[0] + (v)[0], (u)[1] + (v)[1], (u)[2] + (v)[2])
/**
 * Subtracts two vectors and takes the result into another vector.
 * @param u,v Input vectors
 * @param r   Result vector
 */
#define vector_sub(u, v, r) \
  vector_set((r), (u)[0] - (v)[0], (u)[1] - (v)[1], (u)[2] - (v)[2])
/**
 * Multiplies a vector by a scalar value.
 * @param s Scalar
 * @param v Input vector
 * @param r Result vector
 */
#define vector_mul(s, v, r) \
  vector_set((r), ((v)[0])*(s), ((v)[1])*(s), ((v)[2])*(s))
/**
 * Multiplies two vectors and takes the result into another vector.
 * @param u,v Input vectors
 * @param r   Result vector
 */
#define vector_mulv(u, v, r) \
  vector_set((r), (u)[0] * (v)[0], (u)[1] * (v)[1], (u)[2] * (v)[2])

/**
 * Returns the dot product of two vectors.
 * @param u,v Input vectors
 * @return Dot product of <u, v>
 */
#define vector_dot(u, v) \
  (((u)[0])*((v)[0]) + ((u)[1])*((v)[1]) + ((u)[2])*((v)[2]))
/**
 * Returns the dot product of two vectors clamped between 0 and 1.
 * @param u,v Input vectors
 * @return Dot product of <u, v>
 */
#define vector_dot_(u, v) \
  fmax(fmin(vector_dot(u, v), 1.0f), 0.0f)
/**
 * Gets the cross product of two vectors.
 * @param u,v Input vectors
 * @param r   Result vector
 */
#define vector_cross(u, v, r) \
  vector_set((r), \
    ((u)[1])*((v)[2]) - ((u)[2])*((v)[1]), \
    ((u)[2])*((v)[0]) - ((u)[0])*((v)[2]), \
    ((u)[0])*((v)[1]) - ((u)[1])*((v)[0]))

/**
 * Returns the length or magnitude of a vector
 * @param v Vector
 * @return Length/magnitude of v
 */
#define vector_length(v) \
  sqrtf(vector_dot(v, v))
/**
 * Clamps the values of v between min and max
 * @param v       Vector
 * @param min,max Range for the clamping
 * @param r       Result vector
 */
#define vector_clamp(v, min, max, r) \
  v[0] = fmax(fmin(((v)[0]), max), min); \
  v[1] = fmax(fmin(((v)[1]), max), min); \
  v[2] = fmax(fmin(((v)[2]), max), min)

/**
 * Normalizes a vector.
 * @param v Vector
 * @param r Result vector
 */
#define vector_normalize(v, r) { \
  float _l = 1 / vector_length(v); \
  vector_mul(_l, v, r); \
}

/**
 * Returns the length of the distance between two vectors
 * @param u,v Input vectors
 */
float vector_distance(const VECTOR u, const VECTOR v);

/**
 * Gets the projection of the vector u into the vector v
 * @param u,v Input vectors
 * @param r   Result vector
 */
float* vector_proj(const VECTOR u, const VECTOR v, VECTOR r);

/**
 * Applies an unary operator to every coordinate of a vector.
 * @param u        Input vector
 * @param function Unary operator
 * @param r        Result vector
 */
float* vector_function(const VECTOR u, float(*function)(float), VECTOR r);
/**
 * Applies a binary operator to every coordinate of a vector.
 * @param u        Input vector
 * @param function Binary operator
 * @param r        Result vector
 */
float* vector_function2(const VECTOR u, const VECTOR v, float(*function)(float, float), VECTOR r);

#endif
