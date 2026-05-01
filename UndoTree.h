#ifndef _UNDO_TREE_H_
#define _UNDO_TREE_H_

#include <corecrt.h>

#include "Painting.h"
#include "Tools.h"

#define MAX_UNDOS 10

typedef struct UndoState UndoState;


typedef struct UndoStateList {
	UndoState *p;
	int len;
	int cap;
} UndoStateList;

typedef struct UndoState {
  Painting state;
  ToolCode toolUsed;
  UndoState *parentState;
  UndoStateList childStates;
} UndoState;

typedef struct UndoTree {
  UndoState root;
  UndoState *curState;
} UndoTree;

errno_t UndoTreeCreate(UndoTree *tree);

errno_t UndoTreeAddState(UndoTree *tree, Painting painting, ToolCode toolUsed);

errno_t UndoTreePopState(UndoTree *tree, Painting *ret);

void UndoTreeDestroy(UndoTree *tree);

void UndoStateDestroyAndPainting(UndoState *state);

void UndoStateDestroy(UndoState *state);

#endif
