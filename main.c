#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "vector.h"
#include "scene.h"
#include "ray.h"
#include "material.h"

#define WIDTH  400
#define HEIGHT WIDTH

#define RESOLUTION 1
#define ANTIALIAS  2

static float material1[] = {
  //0.9f, 0.5f, 0.7f, // diffuse color
  0.7f, 0.7f, 0.7f, // diffuse color
  6.0f,             // diffuse coefficient
  0.5f, 0.5f, 0.5f, // specular color
  300.0f            // specular coefficient
};
static float material2[] = {
  1.0f, 0.0f, 0.0f, // diffuse color
  2.0f,             // diffuse coefficient
  0.5f, 0.5f, 0.5f, // specular color
  400.0f            // specular coefficient
};
static float material3[] = {
  0.75f, 0.8f, 0.6f,// diffuse color
  //1.0f, 1.0f, 1.0f,// diffuse color
  5.0f,             // diffuse coefficient
};
static float plane_material[] = {
  0.88f, 0.88f, 1.0f, // diffuse color
  1.5f,               // diffuse coefficient
};

static float sphere1_points[] = {
  0.4f, -0.1f, 0.8f,
  0.4f
};
static float sphere2_points[] = {
  -0.25f, -0.3f, 0.5f,
  0.2f
};

static float tethraedron_points[] = {
  -1.5f,  2.5f, 5.0f,
  -0.5f, -0.5f, 0.2f,
  -5.5f, -0.5f, 10.5f,
  -0.2f, -0.5f, 1.7f
};
static size_t tethraedron_indices[] = {
  4,
  0, 1, 2,
  0, 2, 3,
  0, 3, 1,
  1, 3, 2
};

static float cube_points[] = {
  -1.0f, -0.5f, 9.0f,
   1.0f, -0.5f, 9.0f,
   1.0f,  1.5f, 9.0f,
  -1.0f,  1.5f, 9.0f,
  -1.0f, -0.5f, 7.0f,
   1.0f, -0.5f, 7.0f,
   1.0f,  1.5f, 7.0f,
  -1.0f,  1.5f, 7.0f
};
static size_t cube_indices[] = {
  12,
  0, 1, 2,  0, 2, 3, // back
  0, 5, 1,  1, 4, 5, // top
  4, 6, 5,  4, 7, 6, // front
  7, 2, 6,  7, 3, 2, // bottom
  0, 7, 4,  0, 3, 7, // left
  5, 2, 1,  5, 6, 2 // right,
};

static float plane_points[] = {
   0.0f, -0.5f, 0.0f, /**< plane origin */
   0.0f,  1.0f, 0.0f  /**< plane normal vector */
};


static SOLID solids[] = {
  { 1, sphere1_points, NULL, NULL, {0.5f, material1, NULL, LAMBERT}, SPHERE },
  { 1, sphere2_points, NULL, NULL, {0.12f, material2, NULL, PHONG}, SPHERE },
  { 4, tethraedron_points, NULL, tethraedron_indices, {0.1f, material3, NULL, LAMBERT}, TRIANGLE },
  { 6, cube_points, NULL, cube_indices, {0.12f, material3, NULL, LAMBERT}, TRIANGLE },
  { 1, plane_points, NULL, NULL, {0.1f, plane_material, NULL, LAMBERT}, PLANE }
};

static LIGHT lights[] = {
  { DIRECTIONAL, { 0.1f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, 1.2f },
  //{ DIRECTIONAL, { 0.5f, -2.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, 0.7f },
  { DIRECTIONAL, {-0.5f, -2.5f, -3.0f}, {0.75f, 0.8f, 1.0f}, 1.8f }
  //{ POINT, { -0.5f,  2.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, 0.0f },
  //{ POINT, { -0.1f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, 1.0f },
  //{ POINT, { -1.0f,  2.5f,  6.0f}, {0.75f, 0.8f, 1.0f}, 1.8f }
};


static SCENE scene = {
  5,      /* number of solids */
  solids, /* list of solids */
  2,      /* number of lights */
  lights, /* list of lights */
  { 0.1f, 0.1f, 0.1f },  /* ambient color */
  { 0.55f, 0.55f, 0.7f } /* background color */
};

/**
 * 
 */
int main(int argc, char** argv)
{
  int x, y;
  int xx, yy;

  float color[3];

  float origin[3] = { 0.0f, 0.0f, -1.0f };
  float fragment[6] = {
    0.0f, 0.0f, 0.0f, // position
    0.0f, 0.0f, 0.0f  // color
  };

  // initialize rays with near and far values
  RAY ray = { 0.001f, 1000.0f };

  IMAGE* img = image(WIDTH, HEIGHT);

  IMAGE* tile_texture = image_read("img/tiles.ppm", image_read_ppm);
  solids[0].material.texture = tile_texture;

  //const float translation[3] = { 0.0f, 1.0f, 8.0f };
  //solid_translate(&solids[3], translation);

  // do the raytracing
  for(x = 0; x < WIDTH; x += RESOLUTION)
  for(y = 0; y < HEIGHT; y += RESOLUTION) {

    v_set(color, 0, 0, 0);

    // anti-aliasing iteration
    for(xx = 0; xx < ANTIALIAS; xx++)
    for(yy = 0; yy < ANTIALIAS; yy++) {
      // calculate ray
      fragment[0] = (x + (float)xx/ANTIALIAS)/WIDTH - 0.5f;
      fragment[1] = 0.5f - (y + (float)yy/ANTIALIAS)/HEIGHT;
      fragment[2] = 0.0f;
      ray_calculate(&ray, origin, fragment, false);
      
      ray_trace(&ray, &scene, &fragment[3]);
      v_add(color, &fragment[3], color);
    }

    // converts color to the [0, 255] range
    v_mul(255.0f*powf(ANTIALIAS, -2), color, color);
    v_clamp(color, 0, 255, color);

    // write to the image
    #if RESOLUTION == 1
    image_setpixel(img, x, y, color[0], color[1], color[2]);
    #else
    image_setpixels_square(img, x, y, RESOLUTION, color[0], color[1], color[2]);
    #endif
  }

  // save the image
  image_write(img, argv[1] != NULL ? argv[1] : "img/test.ppm", image_write_ppm);
  image_free(img);

  return 0;
}
