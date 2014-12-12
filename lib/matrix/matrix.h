/**
 * Defines 3D Matrix algebra and manipulation macros and functions.
 * All of the matrix algebra functions and macros take as last argument
 * the matrix where to store the result of the operation.
 */
#ifndef MATRIX_H_
#define MATRIX_H_

#include <math.h>

#define MATRIX float*

/*
Matrix:
  _u_v_w_
  |0|3|6|
  |1|4|7|
  |2|5|8|
  ¨¨¨¨¨¨¨
*/

// Aliases for matrix functions and macros using the v_ prefix.
#define m_copy matrix_copy
#define m_set matrix_set
#define m_add matrix_add
#define m_sub matrix_sub
#define m_mul matrix_mul
#define m_mulm matrix_mulm

/**
 * Copies the value of a vector to another vector
 * @param A Destination matrix
 * @param B Source matrix
 */
#define matrix_copy(A, B) \
  memcpy(A, B, sizeof(float)*3);

/**
 * Sets a matrix components.
 * @param A     Matrix
 * @param u,v,w Vectors
 */
#define matrix_set(A, u, v, w) \
  v_copy(&A[0], u); \
  v_copy(&A[3], v); \
  v_copy(&A[6], w)

/**
 * Adds two matrices and takes the result into another matrix.
 * @param A,B Input matrices
 * @param M   Result matrix
 */
#define matrix_add(A, B, M) \
  matrix_set((A), (u)[0] + (v)[0], (u)[1] + (v)[1], (u)[2] + (v)[2])
/**
 * Subtracts two matrices and takes the result into another matrix.
 * @param u,v Input matrices
 * @param r   Result matrix
 */
#define matrix_sub(u, v, r) \
  matrix_set((r), (u)[0] - (v)[0], (u)[1] - (v)[1], (u)[2] - (v)[2])
/**
 * Multiplies a matrix by a scalar value.
 * @param s Scalar
 * @param v Input matrix
 * @param r Result matrix
 */
#define matrix_mul(s, v, r) \
  matrix_set((r), ((v)[0])*(s), ((v)[1])*(s), ((v)[2])*(s))
/**
 * Multiplies two matrices and takes the result into another matrix.
 * @param u,v Input matrices
 * @param r   Result matrix
 */
#define matrix_mulm(u, v, r) \
  matrix_set((r), (u)[0] * (v)[0], (u)[1] * (v)[1], (u)[2] * (v)[2])

/**
 * Copies the value of a matrix to another matrix
 * @param u Result matrix
 * @param v Vector to be copied
 */
void matrix_copy(VECTOR u, const VECTOR v);

#endif
