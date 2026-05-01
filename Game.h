#ifndef _GAME_H_
#define _GAME_H_

#include <corecrt.h>

#include "Painting.h"
#include "Tools.h"
#include "UndoTree.h"

typedef struct Game {
  UndoTree tree;
  Painting canvas;
  Tool tool;
  Color color;
  bool startedPlacing;
  int startX, startY, endX, endY;
} Game;

// In case the user wants to control the loop
errno_t GameTick(Game *game);

errno_t GameInit(Game *game);

errno_t GameRun(Game *game);

void GameDestroy(Game *game);

#endif
