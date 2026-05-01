#ifndef _PAINTING_H_
#define _PAINTING_H_

#include <corecrt.h>
#include <raylib.h>

typedef struct Painting {
  int width;
  int height;
  Color *pixels;
} Painting;

Painting PaintingNew(const int width, const int height);

void PaintingDestroy(Painting *painting);

void PaintingDraw(Painting *painting);

void PaintingDrawRect(Painting *painting, int sx, int sy, int ex, int ey, const Color color);

void PaintingDrawLine(Painting *painting, const int sx, const int sy, const int ex, const int ey, const Color color);

void PaintingDrawCircle(Painting *painting, int sx, int sy, int ex, int ey, const Color color);

void PaintingFill(Painting *painting, const Color color);

errno_t PaintingCopy(Painting *dst, const Painting src);

#endif
