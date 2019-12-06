#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ITERATIONS 10
#define BURST 10000

int main()
{
    int descr[2];

    int flag = O_RDONLY;

    assert(0 == pipe(descr));

    /* consumer */
    for (int i = 0; i < ITERATIONS; i++)
    {
        for (int j = 0; j < BURST; j++)
        {
            int buffer[10];
            read(descr[0], &buffer, 10);
        }
        sleep(1);
    }
    printf("consumer done\n");
    return 0;
}