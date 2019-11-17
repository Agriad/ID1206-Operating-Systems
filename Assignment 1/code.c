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
    int pid = getpid();

    unsigned long p = 0x1;

    foo(&p);

    back:
        printf("  p: %p \n", &p);
        printf("  back: %p \n", &&back);

        printf("\n\n /proc/%d/maps", pid);
        char command[50];
        sprintf(command, "cat /proc/%d/maps", pid);
        system(command);

    return 0;

    /*
    printf("  p (0x%lx ): %p \n", p, &p);

    printf("\n\n /proc/%d/maps \n\n", pid);
    char command[50];
    sprintf(command, "cat /proc/%d/maps", pid);
    system(command);

    return 0;
    */

    /*
    foo:
        printf("process id: %d\n", pid);
        printf("global string: %p\n", &global);
        printf("the code: %p\n", &&foo);
        printf("global constant: %p\n", &onion);

        printf("\n\n /proc/%d/maps", pid);
        char command[50];
        sprintf(command, "cat /proc/%d/maps", pid);
        system(command);

        return 0;
    */
}