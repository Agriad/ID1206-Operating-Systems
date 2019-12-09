#include <stdio.h>
#include "green_final_touch.h"

int flag = 0;
green_cond_t cond;
green_mutex_t temp;
green_mutex_t *mutex = &temp;
int counter = 0;

void *test(void *arg)
{
    int id = *(int*)arg;
    int loop = 100000;
    while (loop > 0)
    {
        green_mutex_lock(mutex);
        while (flag != id)
        {
            //printf("while\n");
            green_cond_wait(&cond, mutex);
        }
        //printf("thread %d: %d\n", id, loop);
        flag = (id + 1) % 10;
        green_cond_signal(&cond);
        green_mutex_unlock(mutex);
        loop--;
        counter++;
    }
}

int main()
{
    //green_t g0, g1, g2;
    green_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9;
    int a0 = 0;
    int a1 = 1;
    int a2 = 2;
    int a3 = 3;
    int a4 = 4;
    int a5 = 5;
    int a6 = 6;
    int a7 = 7;
    int a8 = 8;
    int a9 = 9;
    green_cond_init(&cond);
    green_mutex_init(mutex);
    green_create(&g0, test, &a0);
    green_create(&g1, test, &a1);
    green_create(&g2, test, &a2);
    green_create(&g3, test, &a3);
    green_create(&g4, test, &a4);
    green_create(&g5, test, &a5);
    green_create(&g6, test, &a6);
    green_create(&g7, test, &a7);
    green_create(&g8, test, &a8);
    green_create(&g9, test, &a9);

    green_join(&g0, NULL);
    green_join(&g1, NULL);
    green_join(&g2, NULL);
    green_join(&g3, NULL);
    green_join(&g4, NULL);
    green_join(&g5, NULL);
    green_join(&g6, NULL);
    green_join(&g7, NULL);
    green_join(&g8, NULL);
    green_join(&g9, NULL);

    printf("counter: %d\n", counter);
    printf("done\n");
    return 0;
}