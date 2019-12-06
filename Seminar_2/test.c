#include "dlmall.h"

int main()
{
    void *arena = new();
    insert(arena);
    sanity();
    /*
    printf("dalloc 100\n");
    void *mem = dalloc(100);
    sanity();
    printf("pfree 100\n");
    pfree(mem);
    sanity();
    */

    /*
    printf("dalloc 100\n");
    void *mem = dalloc(100);
    sanity();
    printf("dalloc 150\n");
    void *mem1 = dalloc(150);
    sanity();
    printf("pfree 150\n");
    pfree(mem1);
    sanity();
    printf("pfree 100\n");
    pfree(mem);
    sanity();
    */


    printf("dalloc 100\n");
    void *mem = dalloc(100);
    sanity();
    printf("pfree 100\n");
    pfree(mem);
    sanity();
    printf("dalloc 150\n");
    void *mem1 = dalloc(150);
    sanity();
    printf("pfree 150\n");
    pfree(mem1);
    sanity();
    printf("dalloc 90\n");
    void *mem2 = dalloc(90);
    sanity();
    printf("pfree 90\n");
    pfree(mem2);
    sanity();
    printf("dalloc 250\n");
    void *mem3 = dalloc(250);
    sanity();
    printf("pfree 250\n");
    pfree(mem3);
    sanity();
    printf("dalloc 300\n");
    void *mem4 = dalloc(300);
    sanity();
    printf("pfree 300\n");
    pfree(mem4);
    sanity();

    return 0;
}