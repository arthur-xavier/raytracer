/**
 * Defines an ADT for Image.
 */
#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdio.h>
#include <sys/types.h>

/**
 * Alias for (unsigned char).
 */
typedef unsigned char u_char;

/**
 * Image data type, contains its width, height and pixel data.
 */
typedef struct IMAGE {
  int width;
  int height;
  u_int** data; /* stores pixel data as an u_int representing 3 u_char {r, g, b} */
} IMAGE;

/**
 * Creates an empty image for a given width and height.
 * @param width Width of the image
 * @param height Height of the image
 * @return Pointer to the allocated struct
 */
IMAGE* image(int width, int height);

/**
 * Destroys an Image pointer and its pixel data.
 * @param img Image to be destroyed
 */
void image_free(IMAGE* img);

/**
 * Sets a pixel of the image to the specified color.
 * @param img   Image pointer
 * @param x,y   Coordinates of the pixel
 * @param r,g,b Pixel color
 */
void image_setpixel(IMAGE* img, int x, int y, u_char r, u_char g, u_char b);

/**
 * Sets a squared group of pixels of the image to the specified color.
 * @param img   Image pointer
 * @param x,y   Coordinates of the pixel
 * @param n     Length of the pixel square
 * @param r,g,b Pixel color
 */
void image_setpixels_square(IMAGE* img, int x, int y, size_t n, u_char r, u_char g, u_char b);

/**
 * Returns the color of a pixel.
 * @param img   Image pointer
 * @param x,y   Coordinates of the pixel
 * @param r,g,b Color pointers. Are set according to the pixel color
 */
void image_getpixel(IMAGE* img, int x, int y, u_char* r, u_char* g, u_char* b);

/**
 * Returns the color of a pixel.
 * @param img Image pointer
 * @param x,y Coordinates of the pixel
 * @param rgb Color array. Is set according to the pixel color
 */
void image_getpixelv(IMAGE* img, int x, int y, u_char* rgb);

/**
 * Returns the color of a pixel as a float vector.
 * @param img Image pointer
 * @param x,y Coordinates of the pixel
 * @param rgb Floating point color array. Between 0.0f and 1.0f
 */
void image_getpixelf(IMAGE* img, int x, int y, float* rgb);

/**
 * Reads an image from a source file with a reading function.
 * @param filename Name of the image file
 * @param read     Reading function pointer
 * @return Image pointer read
 */
IMAGE* image_read(char* filename, void(*read)(FILE*, IMAGE*));
/**
 * Writes an image to a source file with a writing function.
 * @param img      Image pointer to be written
 * @param filename Name of the image file
 * @param write    Write function pointer
 */
void image_write(IMAGE* img, char* filename, void(*write)(FILE*, IMAGE*));

/**
 * Reads an image from a PPM source file.
 * @param file File pointer
 * @param img  Image pointer
 */
void image_read_ppm(FILE* file, IMAGE* img);
/**
 * Writes an image to a PPM source file.
 * @param file File pointer
 * @param img  Image pointer
 */
void image_write_ppm(FILE* file, IMAGE* img);

#endif
