#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "image.h"

IMAGE* image(int width, int height) {
  int y;
  IMAGE* img = (IMAGE*)malloc(sizeof(IMAGE));
  img->width = width;
  img->height = height;

  img->data = (u_int**)malloc(sizeof(u_int*) * img->height);

  for(y = 0; y < img->height; y++) {
    img->data[y] = (u_int*)malloc(sizeof(u_int)*img->width);
    memset(img->data[y], 0, sizeof(u_int)*img->width);
  }

  return img;
}
void image_free(IMAGE* img) {
  int y;
  for(y = 0; y < img->height; y++) {
    free(img->data[y]);
  }
  free(img->data);
  free(img);
}

void image_setpixel(IMAGE* img, int x, int y, u_char r, u_char g, u_char b) {
  img->data[y][x] = (r << 16) | (g << 8) | b;
}
void image_setpixels_square(IMAGE* img, int x, int y, size_t n, u_char r, u_char g, u_char b) {
  int yy = y + n;
  int xx = x + n;
  u_int p = (r << 16) | (g << 8) | (u_int)b;
  for(; y < yy; y++) {
    for(x = xx - n; x < xx; x++)
      img->data[y][x] = p;
  }
}
void image_getpixel(IMAGE* img, int x, int y, u_char* r, u_char* g, u_char* b) {
  *r = (img->data[y][x] & 0xFF0000) >> 16;
  *g = (img->data[y][x] & 0xFF00) >> 8;
  *b = img->data[y][x] & 0xFF;
}
void image_getpixelv(IMAGE* img, int x, int y, u_char* rgb) {
  image_getpixel(img, x, y, &rgb[0], &rgb[1], &rgb[2]);
}
void image_getpixelf(IMAGE* img, int x, int y, float* rgb) {
  rgb[0] = (float)((img->data[y][x] & 0xFF0000) >> 16) / 255.0f;
  rgb[1] = (float)((img->data[y][x] & 0xFF00) >> 8) / 255.0f;
  rgb[2] = (float)(img->data[y][x] & 0xFF) / 255.0f;
}

IMAGE* image_read(char* filename, void(*read)(FILE*, IMAGE*)) {
  FILE* file;
  IMAGE* img;

  file = fopen(filename, "r");

  if(file) {
    img = (IMAGE*)malloc(sizeof(IMAGE));
    read(file, img);
  } else {
    fprintf(stderr, "Error while opening file '%s'", filename);
    exit(1);
  }
  return img;
}

void image_write(IMAGE* img, char* filename, void(*write)(FILE*, IMAGE*)) {
  FILE* file;

  file = fopen(filename, "w");

  if(file) {
    write(file, img);
  } else {
    fprintf(stderr, "Error while opening file '%s'", filename);
    exit(1);
  }
}

void image_read_ppm(FILE* file, IMAGE* img) {
  int x, y;
  u_char r, g, b;
  int max;
  char buffer[255];

  fgets(buffer, 255, file); // P6
  fgets(buffer, 255, file); // HEADER
  fscanf(file, "%d %d", &img->width, &img->height);
  fscanf(file, "%d", &max);
  fscanf(file, "%c", buffer);

  img->data = (u_int**)malloc(sizeof(u_int*) * img->height);

  for(y = 0; y < img->height; y++) {
    img->data[y] = (u_int*)malloc(sizeof(u_int) * img->width);
    for(x = 0; x < img->width; x++) {
      fscanf(file, "%c%c%c", &r, &g, &b);
      image_setpixel(img, x, y, r, g, b);
    }
  }
}
void image_write_ppm(FILE* file, IMAGE* img) {
  int x, y;
  u_int p;

  fprintf(file, "P6\n# RAYTRACER\n%d %d 255\n", img->width, img->height);
  for(y = 0; y < img->height; y++) {
    for(x = 0; x < img->width; x++) {
      p = img->data[y][x];
      fprintf(file, "%c%c%c", p >> 16, (p >> 8)&0xFF, p&0xFF);
    }
  }
}