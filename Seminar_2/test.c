#include "dlmall.h"

int main()
{
    void *arena = new();
    insert(arena);
    sanity();
    void *mem = dalloc(8);
    sanity();
    void *mem1 = dalloc(16);
    sanity();
    pfree(mem);
    sanity();
    pfree(mem1);
    sanity();
    dalloc(14);
    sanity();

    return 0;
}