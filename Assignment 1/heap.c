#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "This is a global string";
const int onion[] = {1, 2};

void zot(unsigned long *stop)
{
    unsigned long r = 0x3;

    unsigned long *i;
    for(i = &r; i <= stop; i++)
    {
        printf(" %p     0x%lx \n", i, *i);
    }
}

void foo(unsigned long *stop)
{
    unsigned long q = 0x2;

    zot(stop);
}

int main()
{
    /*
    char *heap = malloc(20);
    *heap = 0x61;
    printf ("heap pointing to: 0x%x\n", *heap);
    free(heap);
    */

    long *heap = (unsigned long*) calloc(40, sizeof(unsigned long));

    printf("heap[2]:   0x%lx\n", heap[2]);
    printf("heap[1]:   0x%lx\n", heap[1]);
    printf("heap[0]:   0x%lx\n", heap[0]);
    printf("heap[-1]:  0x%lx\n", heap[-1]);
    printf("heap[-2]:  0x%lx\n", heap[-2]);

    free(heap);
    printf("heap[2]:   0x%lx\n", heap[2]);
    printf("heap[1]:   0x%lx\n", heap[1]);
    printf("heap[0]:   0x%lx\n", heap[0]);
    printf("heap[-1]:  0x%lx\n", heap[-1]);
    printf("heap[-2]:  0x%lx\n", heap[-2]);

    char *foo = malloc(20);
    *foo = 0x62;
    printf("foo pointing to: 0x%x\n", *foo);

    /*danger ahead*/
    *heap = 0x63;
    printf("or is it pointing to: 0x%x\n", *foo);

    int pid = getpid();


    unsigned long p = 0x1;

    /*foo(&)*/

    back:
        /*printf("  p: %p \n", &p);*/
        printf("  back: %p \n", &&back);

        printf("\n\n /proc/%d/maps", pid);
        char command[50];
        sprintf(command, "cat /proc/%d/maps", pid);
        system(command);

    return 0;

    /*
    char *heap = malloc(20);

    printf("the heap variable at: %p\n", &heap);
    printf("pointing to: %p\n", heap);
    
    return 0;
    */
}