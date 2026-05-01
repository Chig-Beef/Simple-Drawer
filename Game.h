#ifndef _GAME_H_
#define _GAME_H_

#include <corecrt.h>

#include "Painting.h"
#include "Tools.h"
#include "UndoTree.h"

typedef struct Game {
  UndoTree tree;
  Painting canvas;
  ToolCode tool;
  Color color;
  bool startedPlacing;
  int startX, startY, endX, endY;
} Game;

errno_t GameTick(Game *game);

#endif
