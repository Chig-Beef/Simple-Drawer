#include <corecrt.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "UndoTree.h"

errno_t UndoStateListCreate(UndoStateList *list, const int cap) {
	list->cap = cap;
	list->len = 0;
	list->p = malloc(cap*sizeof(UndoState));
	return list->p == NULL;
}

void UndoStateListDestroy(UndoStateList *list) {
	if (list->cap == 0) return;
	free(list->p);
}

errno_t UndoStateGrow(UndoStateList *list) {
	const int oldCap = list->cap;

	list->cap *= 2;
	if (list->cap == 0) list->cap = 1;

	UndoState *new = malloc(list->cap*sizeof(UndoState));
	if (!new) return 1;

	memcpy(new, list->p, oldCap*sizeof(UndoState));
	free(list->p);
	list->p = new;

	return 0;
}

errno_t UndoStateAppend(UndoStateList *list, UndoState val) {
	if (list->len == list->cap) {
		if (UndoStateGrow(list)) return 1;
	}

	list->p[list->len++] = val;
	return 0;
}

errno_t UndoStateGet(UndoStateList *list, UndoState *out, const int i) {
	if (i < 0 || i >= list->len) return 1;
	*out = list->p[i];
	return 0;
}

int UndoStateIndexOf(UndoStateList *list, UndoState *match) {
	for (int i = 0; i < list->len; ++i) {
		if (&list->p[i] == match) {
			return i;
		}
	}
	return -1;
}

errno_t UndoStateRemoveAt(UndoStateList *list, const int i) {
	if (i < 0 || i >= list->len) return 1;

	list->len--;

	for (int j = i; j < list->len; ++j) {
		list->p[j] = list->p[j+1];
	}

	return 0;
}

errno_t UndoStateRemove(UndoStateList *list, UndoState *match) {
	const int i = UndoStateIndexOf(list, match);
	return UndoStateRemoveAt(list, i);
}

errno_t UndoTreeCreate(UndoTree *tree) {
	tree->curState = &tree->root;
	return UndoStateListCreate(&tree->root.childStates, 1);
}

errno_t UndoTreeAddState(UndoTree *tree, Painting painting, ToolCode toolUsed) {
	UndoState *topState = tree->curState;

	const int newChildIndex = topState->childStates.len;

	// Create the new state object
	UndoState newState;
	newState.parentState = topState;
	errno_t err = UndoStateListCreate(&newState.childStates, 1);
	if (err) {
		printf("Couldn't create child list\n");
		return 1;
	}
	newState.state = painting;
	newState.toolUsed = toolUsed;

	// Add it to the top state's children
	err = UndoStateAppend(&topState->childStates, newState);
	if (err) {
		printf("Couldn't add to child list\n");
		return 1;
	}
	
	tree->curState = topState->childStates.p+newChildIndex;
	return 0;
}

errno_t UndoTreePopState(UndoTree *tree, Painting *ret) {
	UndoState *topState = tree->curState;

	// This is the last undo
	if (topState == &tree->root) {
		printf("Last undo!\n");
		Painting out;
		errno_t err = PaintingCopy(&out, topState->state);
		if (err) {
			printf("Couldn't copy painting\n");
			return 1;
		}
		*ret = out;
		return 0;
	}

	// Pop one off
	tree->curState = topState->parentState;

	// Return the given canvas
	Painting out;
	errno_t err = PaintingCopy(&out, tree->curState->state);
	if (err) {
		printf("Couldn't copy painting\n");
		return 1;
	}

	*ret = out;
	return 0;
}

void UndoStateDestroyAndPainting(UndoState *state) {
	// for (int i = 0; i < state->numChildStates; ++i) {
	// 	UndoStateDestroyAndPainting(&state->childStates[i]);
	// }
	// free(state->childStates);
	// PaintingDestroy(&state->state);
}

void UndoStateDestroy(UndoState *state) {
	for (int i = 0; i < state->childStates.len; ++i) {
		UndoStateDestroy(&state->childStates.p[i]);
	}
	UndoStateListDestroy(&state->childStates);
}

void UndoTreeDestroy(UndoTree *tree) {
	UndoStateDestroy(&tree->root);
}
