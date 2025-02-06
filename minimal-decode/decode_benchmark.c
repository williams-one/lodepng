// gcc lodepng.c decode_benchmark.c -Wall -Wextra -pedantic -O3 -o decode_benchmark && ./decode_benchmark
#include "lodepng.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #include "external_1_800x480.h"
// #include "external_1_400x240.h"
#include "external_1_200x120.h"
// #include "octocat_32bpp.h"
// #include "octocat_32bpp_resampled.h"

#define IMAGE_NAME external_1_200x120_map
// #define IMAGE_NAME octocat_32bpp_map
// #define IMAGE_NAME octocat_32bpp_resampled_map

unsigned char* minimal_decode() {
  unsigned error;
  unsigned char* image = 0;
  unsigned width, height;

  // error = lodepng_decode32(&image, &width, &height, IMAGE_NAME, sizeof(IMAGE_NAME));

  LodePNGState state;
  lodepng_state_init(&state);
  state.info_raw.colortype = LCT_RGBA;
  state.info_raw.bitdepth = 8;
  state.decoder.zlibsettings.ignore_adler32 = 1;
  state.decoder.ignore_crc = 1;
  error = lodepng_decode(&image, &width, &height, &state, IMAGE_NAME, sizeof(IMAGE_NAME));
  lodepng_state_cleanup(&state);

  if (error)
    printf("error %u: %s\n", error, lodepng_error_text(error));

  return image;
}

int main() {
  const int count = 1000;

  double total_time = 0.0;
  for (int i = 0; i < count; i++) {
    clock_t start = clock();
    unsigned char* image = minimal_decode();
    total_time += (double)(clock() - start) / CLOCKS_PER_SEC;
    free(image);
  }
  printf("time [ms]: %f\n", total_time / count * 1000);

#ifdef TRACK_ALLOCATION_DETAILS
  extern unsigned long allocation_count;
  extern unsigned long total_allocation_size;
  extern unsigned long max_allocation_size;
  printf("allocation_count: %lu\n", allocation_count);
  printf("total_allocation_size: %lu\n", total_allocation_size);
  printf("average allocation size: %f\n", (double)total_allocation_size / allocation_count);
  printf("max allocation size: %lu\n", max_allocation_size);
#endif
  return 0;
}

