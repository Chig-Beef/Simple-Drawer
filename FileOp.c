#include <stdio.h>
#include <stdlib.h>

#include "def.h"

// Reads an entire file
errno_t FReadWhole(const char filename[], byte **out, long *len) {
	// Open file
	FILE *f;
	if (fopen_s(&f, filename, "rb")) {
		printf("Couldn't open input file\n");
		return 1;
	}

	// Find size of file
	fseek(f, 0L, SEEK_END);
	long size = ftell(f);
	rewind(f);

	// Allocate enough space to hold whole file
	byte *text = calloc(size, sizeof(byte));

	printf("Read %lli bytes\n", fread(text, sizeof(byte), size, f));

	fclose(f);

	*out = text;
	*len = size;

	return 0;
}


// Writes an entire file
errno_t FWriteWhole(const char filename[], const byte *out, const int outLen) {
	FILE *f;

	errno_t err = fopen_s(&f, filename, "wb");
	if (err) {
		printf("Couldn't open output file\n");
		return 1;
	}

	fwrite(out, sizeof(byte), outLen, f);
	fclose(f);

	return 0;
}
