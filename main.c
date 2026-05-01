#include <stdio.h>
#include <raylib.h>

#include "FileOp.h"
#include "Game.h"
#include "Painting.h"

int main() {
  printf("Starting...\n");

  Game game;
  game.tool = TC_RECT;
  game.color = (Color){255, 0, 0, 255};
  game.startedPlacing = false;
  errno_t err = UndoTreeCreate(&game.tree);
  if (err) {
    printf("Couldn't create undo tree\n");
    return 1;
  }

  // Set up raylib
  SetTraceLogLevel(LOG_NONE);
  SetTargetFPS(60);
  InitWindow(640, 320, "Simple drawer");

  // Set up image
  byte *imageFileData;
  long imageFileLen;
  err = FReadWhole("./test.png", &imageFileData, &imageFileLen);
  if (err) { // Create a blank image
    game.canvas = PaintingNew(240, 240);
    // return 1;
  } else { // TODO: Use the image in the file
    game.canvas = PaintingNew(240, 240);
  }

  err = PaintingCopy(&game.tree.root.state, game.canvas);
  if (err) {
    printf("Couldn't copy painting\n");
    return 1;
  }

  while (!WindowShouldClose()) {
    errno_t err = GameTick(&game);
    if (err) {
      break;
    }
  }

  // Close raylib
  CloseWindow();

  // Close image
  PaintingDestroy(&game.canvas);
  UndoTreeDestroy(&game.tree);

  return 0;
}
