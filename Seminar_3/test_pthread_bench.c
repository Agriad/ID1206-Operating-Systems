#include <stdio.h>
#include "green_final_touch.h"
#include <time.h>
#include <pthread.h>

int flag = 0;
pthread_cond_t cond;
pthread_mutex_t mutex;
int counter = 0;

void *test(void *arg)
{
    int id = *(int*)arg;
    int loop = 10;
    while (loop > 0)
    {
        pthread_mutex_lock(&mutex);
        while (flag != id)
        {
            //printf("while\n");
            pthread_cond_wait(&cond, &mutex);
        }
        printf("thread %d: %d\n", id, loop);
        flag = (id + 1) % 2;
        counter++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        loop--;
    }
}

int main()
{
    clock_t start_clock = clock();
    pthread_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9;
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
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&g0, NULL, test, &a0);
    pthread_create(&g1, NULL, test, &a1);
    /*
    pthread_create(&g2, NULL, test, &a2);
    pthread_create(&g3, NULL, test, &a3);
    pthread_create(&g4, NULL, test, &a4);
    pthread_create(&g5, NULL, test, &a5);
    pthread_create(&g6, NULL, test, &a6);
    pthread_create(&g7, NULL, test, &a7);
    pthread_create(&g8, NULL, test, &a8);
    pthread_create(&g9, NULL, test, &a9);
    */
    
    pthread_join(g0, NULL);
    pthread_join(g1, NULL);
    /*
    pthread_join(g2, NULL);
    pthread_join(g3, NULL);
    pthread_join(g4, NULL);
    pthread_join(g5, NULL);
    pthread_join(g6, NULL);
    pthread_join(g7, NULL);
    pthread_join(g8, NULL);
    pthread_join(g9, NULL);
    */

    printf("counter: %d\n", counter);
    printf("done\n");
    clock_t final_time = clock() - start_clock;
    double final_time_read = (double) final_time / CLOCKS_PER_SEC;
    printf("time: %ld\n", final_time);
    printf("time seconds %lf\n", final_time_read);
    
    return 0;
}