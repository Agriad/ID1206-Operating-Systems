#include "dlmall.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <time.h>

int MIN_SIZE = 4;
int MAX_SIZE = 500;
int BLOCK_AMOUNT = 500;
int ALLOCATED_BLOCK = 100;

struct memory_pointer
{
    void *pointer;
    int size;
    struct memory_pointer *next;
};

int main()
{
    clock_t start_clock = clock();
    srand(time(NULL));
    int allocated_counter = 0;
    int total_counter = 0;
    struct memory_pointer *first = NULL;

    void *arena = new();
    insert(arena);

    while (total_counter < BLOCK_AMOUNT)
    {
        int random_size = rand() % (MAX_SIZE + 1 - MIN_SIZE) + MIN_SIZE;
        int coin_flip = rand() % 2;

        if (coin_flip)
        {
            if (allocated_counter < ALLOCATED_BLOCK)
            {
                void *memory_address = dalloc(random_size);
                if (memory_address != NULL)
                {
                    void *address = malloc(sizeof(memory_address));
                    struct memory_pointer *new = address;
                    new -> pointer = memory_address;
                    new -> next = first;
                    new -> size = random_size;
                    first = new;
                }
                else
                {
                    struct memory_pointer *old;
                    while (first != NULL)
                    {
                        old = first;
                        first = old -> next;
                        pfree(old -> pointer);
                        free(old);
                        old = first;
                        allocated_counter--;
                    }
                    total_counter = BLOCK_AMOUNT;
                }
                
                allocated_counter++;
                total_counter++;
            }
            else
            {
                if (first != NULL)
                {
                    struct memory_pointer *old;
                    old = first;
                    first = old -> next;
                    pfree(old -> pointer);
                    free(old);
                    old = NULL;
                    allocated_counter--;
                }
                else
                {
                    printf("CANNOT FREE ANYMORE BLOCKS\n");
                }
            }
        }
        else
        {
            if (first != NULL)
            {
                struct memory_pointer *old;
                old = first;
                first = old -> next;
                pfree(old -> pointer);
                free(old);
                old = NULL;
                allocated_counter--;
            }
        }
    }

    clock_t final_time = clock() - start_clock;
    double final_time_read = (double) final_time / CLOCKS_PER_SEC;

    another_data();
    printf("total counter: %d allocated counter: %d\n", total_counter, allocated_counter);
    printf("time: %ld\n", final_time);
    printf("time seconds %lf\n", final_time_read);

    return 0;
}