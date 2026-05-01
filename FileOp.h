#include <stdio.h>
#include <stdlib.h>

#include "def.h"

// Reads an entire file
errno_t FReadWhole(const char filename[], byte **out, long *len);

// Writes an entire file
errno_t FWriteWhole(const char filename[], const byte *out, const int outLen);
