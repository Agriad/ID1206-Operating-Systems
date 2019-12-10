#include <stdio.h>
#include "green.h"

int counter = 0;

void *test(void *arg)
{
    int i = *(int*)arg;
    int loop = 100000;
    while (loop > 0)
    {
        printf("thread %d: %d\n", i, loop);
        loop--;
        counter++;
        if (loop % 100 == 0)
        {
            green_yield();
        }
        
    }
}

int main()
{
    green_t g0, g1;
    int a0 = 0;
    int a1 = 1;
    green_create(&g0, test, &a0);
    green_create(&g1, test, &a1);

    green_join(&g0, NULL);
    green_join(&g1, NULL);
    printf("counter: %d\n", counter);
    printf("done\n");
    return 0;
}