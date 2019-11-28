#include "dlmall.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int MIN_SIZE = 4;
int MAX_SIZE = 500;
int BLOCK_AMOUNT = 500;
int ALLOCATED_BLOCK = 50;

struct memory_pointer
{
    void *pointer;
    int size;
    struct memory_pointer *next;
};


int main()
{
    srand(time(NULL));
    int allocated_counter = 0;
    int total_counter = 0;
    struct memory_pointer *first = NULL;

    void *arena = new();
    insert(arena);
    sanity();

    while (total_counter < BLOCK_AMOUNT)
    {
        printf("total counter: %d allocated counter: %d\n", total_counter, allocated_counter);
        int random_size = rand() % (MAX_SIZE + 1 - MIN_SIZE) + MIN_SIZE;
        int coin_flip = rand() % 2;

        if (coin_flip)
        {
            if (allocated_counter < ALLOCATED_BLOCK)
            {
                void *memory_address = dalloc(random_size);
                if (memory_address != NULL)
                {
                    printf("SETTING MEMORY %p, size %d\n", memory_address, random_size);
                    void *address = malloc(sizeof(memory_address));
                    struct memory_pointer *new = address;
                    new -> pointer = memory_address;
                    new -> next = first;
                    new -> size = random_size;
                    first = new;
                    sanity();
                }
                else
                {
                    printf("SETTING MEMORY %p, size %d FAILED not enough space\n", memory_address, random_size);
                    struct memory_pointer *old;
                    while (first != NULL)
                    {
                        old = first;
                        first = old -> next;
                        printf("FREEING MEMORY %p, size %d\n", old -> pointer, old -> size);
                        pfree(old -> pointer);
                        free(old);
                        old = first;
                        sanity();
                        allocated_counter--;
                        printf("total counter: %d allocated counter: %d\n", total_counter, allocated_counter);
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
                    printf("FREEING MEMORY %p, size %d\n", old -> pointer, old -> size);
                    pfree(old -> pointer);
                    free(old);
                    old = NULL;
                    sanity();
                    allocated_counter--;
                }
                else
                {
                    printf("CANNOT FREE ANYMORE BLOCKS\n");
                    total_counter = BLOCK_AMOUNT;
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
                printf("FREEING MEMORY %p, size %d\n", old -> pointer, old -> size);
                pfree(old -> pointer);
                free(old);
                old = NULL;
                sanity();
                allocated_counter--;
            }
        }
    }

    return 0;
}