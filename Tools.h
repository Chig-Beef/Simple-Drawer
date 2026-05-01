#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "def.h"
#include "stdbool.h"

// Forward declare Game
typedef struct Game Game;

typedef enum ToolCode {
  TC_RECT,
  TC_CURSOR,
  TC_CIRCLE,
  TC_FILL,
  TC_LINE,
  NUM_TOOLS,
} ToolCode;

// The function that the tool does when finished
// (e.g., actually drawing the rectangle)
typedef void (*ToolEndFunc) (Game*);

// The function that determines whether the end func
// should be called
typedef bool (*ToolCheckEndFunc) (Game*);

const char *getToolName(ToolCode tc);
const ToolEndFunc getToolEndFunc(ToolCode tc);
const ToolCheckEndFunc getToolCheckEndFunc(ToolCode tc);

#endif
