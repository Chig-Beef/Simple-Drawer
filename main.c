#include <stdio.h>
#include <raylib.h>

#include "Game.h"

int main() {
  printf("Starting...\n");

  Game game;

  if (GameInit(&game)) {
    printf("Couldn't init game\n");
    return 1;
  }

  // Set up raylib
  SetTraceLogLevel(LOG_NONE);
  SetTargetFPS(60);
  InitWindow(640, 320, "Simple drawer");

  if (GameRun(&game)) {
    return 1;
  }

  // Close raylib
  CloseWindow();

  GameDestroy(&game);

  return 0;
}
