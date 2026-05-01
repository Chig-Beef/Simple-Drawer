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

bool ToolCheckEndDrag(Game *game) {
  if (IsMouseButtonDown(0)) {
    if (!game->startedPlacing) {
      // Start placing
      game->startedPlacing = true;
      game->startX = GetMouseX();
      game->startY = GetMouseY();
    }
  } else {
    if (game->startedPlacing) {
      game->endX = GetMouseX();
      game->endY = GetMouseY();
      game->startedPlacing = false;

      return true;
    }
  }
  return false;
}

bool ToolCheckEndClick(Game *game) {
  return IsMouseButtonPressed(0);
}

const Tool tools[NUM_TOOLS] = {
  {TC_RECT, "rectangle", ToolEndRectangle, ToolCheckEndDrag},
  {TC_CURSOR, "cursor", NULL, NULL},
  {TC_CIRCLE, "circle", ToolEndCircle, ToolCheckEndDrag},
  {TC_FILL, "fill", ToolEndFill, ToolCheckEndClick},
  {TC_LINE, "line", ToolEndLine, ToolCheckEndDrag},
};

const Tool getTool(ToolCode tc) {
  if (tc >= NUM_TOOLS) {
    return (Tool){NUM_TOOLS, NULL, NULL};
  }
  return tools[tc];
}
