#include <raylib.h>
#include <stdio.h>
#include <stddef.h>

#include "Game.h"
#include "Painting.h"
#include "Tools.h"
#include "UndoTree.h"

errno_t GameChangeCanvas(Game *game) {
  const ToolEndFunc endFunc = getToolEndFunc(game->tool);
  if (endFunc == NULL) {
    printf("No canvas altering function found\n");
    return 1;
  }

  // Alter canvas
  endFunc(game);

  // Copy the canvas previous state
  Painting canvasCopy;
  errno_t err = PaintingCopy(&canvasCopy, game->canvas);
  if (err) {
    printf("Couldn't copy current canvas\n");
    return 1;
  }

  // Add previous state to undo tree
  err = UndoTreeAddState(&game->tree, canvasCopy, game->tool);
  if (err) {
    printf("Couldn't add state to tree\n");
    return 1;
  }

  return 0;
}

errno_t GamePerformUndo(Game *game) {
  // Get the last usable state
  Painting prev;
  errno_t err = UndoTreePopState(&game->tree, &prev);
  if (err) {
    printf("Couldn't pop top state\n");
    return 1;
  }

  // Invalid state?
  if (!prev.pixels) {
    printf("Bad state\n");
    return 1;
  }

  Painting old = game->canvas;

  // Place in new canvas
  game->canvas = prev;

  // Destroy old canvas
  PaintingDestroy(&old);

  return 0;
}

errno_t GamePerformRedo(Game *game, const int stateNum) {
  // Top state
  UndoState *topState = game->tree.curState;

  // In bounds?
  if (stateNum < 0 || stateNum >= topState->childStates.len) {
    // Invalid state num
    return 0;
  }

  // Get that state
  UndoState *newState = &topState->childStates.p[stateNum];

  // This is now top state
  game->tree.curState = newState;

  Painting new = newState->state;
  if (!new.pixels) {
    printf("Bad pixels!\n");
    return 1;
  }

  Painting old = game->canvas;

  // Use this canvas
  errno_t err = PaintingCopy(&game->canvas, new);
  if (err) {
    printf("Couldn't copy painting\n");
    return 1;
  }

  // Destroy what we have
  PaintingDestroy(&old);

  return 0;
}

errno_t GameUpdate(Game *game) {
  bool enactEnding = false;
  const ToolCheckEndFunc checkEnd = getToolCheckEndFunc(game->tool);
  if (checkEnd != NULL) {
    enactEnding = checkEnd(game);
  }

  if (enactEnding) {
    errno_t err = GameChangeCanvas(game);
    if (err) {
      return err;
    }
  }

  // Maybe load an undo?
  if (IsKeyPressed(KEY_U)) {
    errno_t err = GamePerformUndo(game);
    if (err) {
      printf("Couldn't perform undo\n");
      return 1;
    }
  }

  // Attempt redo
  if (IsKeyDown(KEY_R)) {
    for (int i = 0; i < 10; ++i) {
      if (IsKeyPressed(KEY_ZERO+i)) {
        errno_t err = GamePerformRedo(game, i);
        if (err) {
          printf("Couldn't perform redo\n");
        }
        break;
      }
    }

    // Switch tool
  } else {
    for (int i = 0; i < NUM_TOOLS; ++i) {
      if (IsKeyPressed(KEY_ZERO+i)) {
        game->tool = (ToolCode)i;
      }
    }
  }

  return 0;
}

errno_t GameDraw(Game *game) {
  BeginDrawing();

  ClearBackground(BLACK);

  // Display the painting
  PaintingDraw(&game->canvas);

  // Display what tool we're using
  DrawText(getToolName(game->tool), 5, 5, 10, WHITE);

  EndDrawing();

  return 0;
}

errno_t GameTick(Game *game) {
  errno_t err;

  err = GameUpdate(game);
  if (err) return err;
  
  err = GameDraw(game);
  if (err) return err;

  return 0;
}
