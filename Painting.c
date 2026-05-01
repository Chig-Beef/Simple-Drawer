#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Painting.h"

Painting PaintingNew(const int width, const int height) {
  Painting painting = (Painting){width, height};

  const int totalPixels = width*height;
  painting.pixels = malloc(totalPixels*sizeof(Color));
  memset(painting.pixels, 0, totalPixels*sizeof(Color));

  return painting;
}

void PaintingDestroy(Painting *painting) {
  free(painting->pixels);
  painting->pixels = NULL;
}

void PaintingDraw(Painting *painting) {
  // Convert to image
  Image imageBuffer;
  imageBuffer.data = painting->pixels;
  imageBuffer.width = painting->width;
  imageBuffer.height = painting->height;
  imageBuffer.mipmaps = 1;
  imageBuffer.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

  // Convert image to texture
  Texture textureBuffer;
  textureBuffer = LoadTextureFromImage(imageBuffer);

  // Draw texture
  DrawTexture(textureBuffer, 0, 0, WHITE);
}

void PaintingDrawRect(Painting *painting, int sx, int sy, int ex, int ey, const Color color) {
  if (sx < 0 || sy < 0 || ex < 0 || ey < 0) return;
  if (sx >= painting->width || ex >= painting->width) return;
  if (sy >= painting->height || ey >= painting->height) return;
  if (sx > ex) {
    const int tmp = sx;
    sx = ex;
    ex = tmp;
  }
  if (sy > ey) {
    const int tmp = sy;
    sy = ey;
    ey = tmp;
  }

  for (int col = sx; col < ex; ++col) {
    for (int row = sy; row < ey; ++row) {
      painting->pixels[row*painting->width+col] = color;
    }
  }
}

void PaintingDrawLine(Painting *painting, const int sx, const int sy, const int ex, const int ey, const Color color) {
  if (sx < 0 || sy < 0 || ex < 0 || ey < 0) return;
  if (sx >= painting->width || ex >= painting->width) return;
  if (sy >= painting->height || ey >= painting->height) return;
  if (sx >= ex || sy >= ey) return;

  const int dx = ex - sx;
  const int dy = ey - sy;
  const int dissqr = dx*dx + dy*dy;
  const double dis = sqrt((double)dissqr);
  const double nx = ((double)dx) / dis;
  const double ny = ((double)dy) / dis;

  int x = sx;
  int y = sy;
  int iter = 0;

  while (x <= ex && y <= ey) {
    // Place a dot
    painting->pixels[y*painting->width+x] = color;

    // Move on
    ++iter;
    x = sx + (nx*((double)iter));
    y = sy + (ny*((double)iter));
  }
}

void PaintingDrawCircle(Painting *painting, int sx, int sy, int ex, int ey, const Color color) {
  if (sx < 0 || sy < 0 || ex < 0 || ey < 0) return;
  if (sx >= painting->width || ex >= painting->width) return;
  if (sy >= painting->height || ey >= painting->height) return;
  if (sx > ex) {
    const int tmp = sx;
    sx = ex;
    ex = tmp;
  }
  if (sy > ey) {
    const int tmp = sy;
    sy = ey;
    ey = tmp;
  }

  // Ensure square
  const int diameter = ex - sx;
  ey = sy + diameter;
  
  // Find radius
  const int radius = diameter / 2;
  const int radiussqr = radius*radius;

  // Center point
  const int cx = sx + radius;
  const int cy = sy + radius;

  for (int col = sx; col < ex; ++col) {
    for (int row = sy; row < ey; ++row) {
      const int dx = col - cx;
      const int dy = row - cy;
      const int dissqr = dx*dx + dy*dy;
      if (dissqr > radiussqr) continue;
      painting->pixels[row*painting->width+col] = color;
    }
  }
}

void PaintingFill(Painting *painting, const Color color) {
  for (int col = 0; col < painting->width; ++col) {
    for (int row = 0; row < painting->height; ++row) {
      painting->pixels[row*painting->width+col] = color;
    }
  }
}

errno_t PaintingCopy(Painting *dst, const Painting src) {
  dst->width = src.width;
  dst->height = src.height;
  const int totalPixels = dst->width*dst->height;

  dst->pixels = malloc(totalPixels*sizeof(Color));
  if (dst->pixels == NULL) {
    printf("Couldn't alloc painting copy\n");
    return 1;
  }

  memcpy(dst->pixels, src.pixels, totalPixels*sizeof(Color));

  return 0;
}
