#include <stdio.h>
#include <stdint.h>

void *dalloc(size_t request);

void pfree(void *memory);

struct head *new();

void insert(struct head *block);

int sanity();
