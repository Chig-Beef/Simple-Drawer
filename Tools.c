#include "Tools.h"
#include "stddef.h"
#include "Painting.h"
#include "Game.h"

void ToolEndRectangle(Game *game) {
  PaintingDrawRect(&game->canvas, game->startX, game->startY, game->endX, game->endY, game->color);
}

void ToolEndLine(Game *game) {
  PaintingDrawLine(&game->canvas, game->startX, game->startY, game->endX, game->endY, game->color);
}

void ToolEndCircle(Game *game) {
  PaintingDrawCircle(&game->canvas, game->startX, game->startY, game->endX, game->endY, game->color);
}

void ToolEndFill(Game *game) {
  PaintingFill(&game->canvas, game->color);
}

const char *toolNames[NUM_TOOLS] = {
  "rectangle",
  "cursor",
  "circle",
  "fill",
  "line",
};

const ToolEndFunc toolEndFuncs[NUM_TOOLS] = {
  ToolEndRectangle,
  NULL,
  ToolEndCircle,
  ToolEndFill,
  ToolEndLine,
};

const char *getToolName(ToolCode tc) {
  if (tc >= NUM_TOOLS) {
    return "INVALID";
  }
  return toolNames[tc];
}

const ToolEndFunc getToolEndFunc(ToolCode tc) {
  if (tc >= NUM_TOOLS) {
    return NULL;
  }
  return toolEndFuncs[tc];
}
