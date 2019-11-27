#include "dlmall.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int MIN_SIZE = 1;
//int MAX_SIZE = 65488;
int BLOCK_AMOUNT = 500;
int ALLOCATED_BLOCK = 100;

int MAX_SIZE = 500;

// ISSUE PFREE() CALLED TWICE ON SAME MEMORY
void memory_operation(int random_size, int coin_flip, int allocated_counter, 
    int total_counter, void *memory_address)
{
    random_size = rand() % (MAX_SIZE + 1 - MIN_SIZE) + MIN_SIZE;
    coin_flip = rand() % 2;
    int is_freed = 0;
    printf("total counter: %d allocated counter: %d\n", total_counter, allocated_counter);

    if (total_counter >= BLOCK_AMOUNT)
    {
        return;
    }
    else
    {
        if (allocated_counter >= ALLOCATED_BLOCK)
        {
            printf("FREEING MEMORY: %p\n", memory_address);
            pfree(memory_address);
            sanity();
            memory_operation(random_size, coin_flip, (allocated_counter - 1), 
                (total_counter), 0);
            return;
        }
        else
        {
            //if (coin_flip)
            if (1)
            {
                memory_address = dalloc(random_size);
                printf("SETTING MEMORY %p\n", memory_address);
                sanity();
                if (allocated_counter == (ALLOCATED_BLOCK - 1))
                {
                    is_freed = 1;
                }
                memory_operation(random_size, coin_flip, (allocated_counter + 1),
                    (total_counter + 1), memory_address);
            }
            if (is_freed != 1 & total_counter != 0)
            {
                printf("FREEING MEMORY: %p\n", memory_address);
                pfree(memory_address);
                sanity();
                return;
            }
            else
            {
                return;
            }
        }
    }
}

int main()
{
    srand(time(NULL));

    void *arena = new();
    insert(arena);
    sanity();

    memory_operation(0, 0, 0, 0, 0);

    return 0;
}