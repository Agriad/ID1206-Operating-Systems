#include <stdio.h>
#include <stdint.h>

void *dalloc(size_t request);

void pfree(void *memory);

struct taken *new();

void insert(struct taken *block);

int sanity();
