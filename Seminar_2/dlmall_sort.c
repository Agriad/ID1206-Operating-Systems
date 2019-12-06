#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define HEAD (sizeof(struct head))
#define MIN(size) (((size) > (8)) ? (size) : (8))
#define LIMIT(size) (MIN(0) + HEAD + size)
#define MAGIC(memory) ((struct head*) memory - 1)
#define HIDE(block) (void*)((struct head*) block + 1)
#define ALIGN 8
#define ARENA (64 * 1024)

struct head
{
    uint16_t bfree; // 2 bytes, the status of block before
    uint16_t bsize; // 2 bytes, the size of block before
    uint16_t free; // 2 bytes, the status of the block
    uint16_t size; // 2 bytes, the size (max 2^16 i.e 64 Ki byte)
    struct head *next; // 8 bytes pointer
    struct head *prev; // 8 bytes pointer
};

struct head *after(struct head *block)
{
    return (struct head*)((char *)(block) + HEAD + block -> size);
}

struct head *before(struct head *block)
{
    return (struct head*)((char *) (block) - block -> bsize - HEAD);
}

struct head *arena = NULL;

struct head *new()
{
    if (arena != NULL)
    {
        printf("one arena already allocated \n");
        return NULL;
    }

    // using mmap, but could have used sbrk
    struct head *new = mmap(NULL, ARENA,
                            PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS,
                            -1, 0);

    if (new == MAP_FAILED)
    {
        printf("mmap failed: error %d\n", errno);
        return NULL;
    }

    /* make room for head and dummy */
    unsigned int size = ARENA - 2 * HEAD;

    new -> bfree = FALSE;
    new -> bsize = 0;
    new -> free = TRUE;
    new -> size = size;

    struct head *sentinel = after(new);

    sentinel -> bfree = TRUE;
    sentinel -> bsize = size;
    sentinel -> free = FALSE;
    sentinel -> size = 0;

    /* this is the only arena we have */
    arena = (struct head*) new;

    return new;
};

struct head *flist;

void sanity()
{
    printf(" --- SANITY CHECK STARTING ---\n");
    // flist
    int flist_counter = 0;
    struct head *pointer = flist;
    while (pointer != NULL)
    {
        int free = pointer -> free;
        int size = pointer -> size;
        printf("flist: %d, address %p, is free: %d, size: %d\n", flist_counter, pointer, free, size);
        flist_counter++;
        pointer = pointer -> next;
        if (flist_counter % 10 == 0)
        {
            //sleep(1);
        }
    }

    int arena_counter = 0;
    struct head *arena_pointer = arena;
    while (arena_pointer -> size != 0)
    {
        int arena_free = arena_pointer -> free;
        int arena_size = arena_pointer -> size;
        int arena_bfree = arena_pointer -> bfree;
        int arena_bsize = arena_pointer -> bsize;
        printf("arena: free: %d, address %p, size: %d, bfree: %d, bsize: %d\n", arena_free, arena_pointer, arena_size, arena_bfree, arena_bsize);
        arena_pointer = after(arena_pointer);
        arena_counter++;
        if (arena_counter % 10 == 0)
        {
            //sleep(1);
        }
    }
    int arena_free = arena_pointer -> free;
    int arena_size = arena_pointer -> size;
    int arena_bfree = arena_pointer -> bfree;
    int arena_bsize = arena_pointer -> bsize;
    printf("sentinel: free: %d, address %p, size: %d, bfree: %d, bsize: %d\n", arena_free, arena_pointer, arena_size, arena_bfree, arena_bsize);
}


void detach(struct head *block)
{
    block -> free = FALSE;
    if(block -> next != NULL)
    {
        if (block -> prev == NULL)
        {
            flist = block -> next;
        }
        struct head *next_head = block -> next;
        next_head -> prev = block -> prev;    
    }
    if (block -> prev != NULL)
    {
        struct head *prev_head = block -> prev;
        prev_head -> next = block -> next;
    }
    if (block -> prev == NULL && block -> next == NULL)
    {
        flist = NULL;
    }
}

void insert(struct head *block)
{
    block -> free = TRUE;
    if (flist == NULL)
    {
        block -> free = TRUE;
        flist = block;
        block -> next = NULL;
        block -> prev = NULL;
    }
    else
    {
        int counter = 0;
        struct head *flist_pointer = flist;
        int block_size = block -> size;
        int flist_pointer_size = flist -> size;

        struct head *flist_other = flist;

        /*
        while(flist_other -> next != NULL)
        {
            counter++;
        }
        */

        // smallest first
        //while (block_size > flist_pointer_size && flist_pointer -> next != NULL)
        while (block_size < flist_pointer_size && flist_pointer -> next != NULL)
        {
            flist_pointer = flist_pointer -> next;            
            flist_pointer_size = flist_pointer -> size;
            counter++;
        }

        /*
        if (block_size > flist_pointer_size && counter < 2)
        {
            flist_pointer -> next = block;
            flist_pointer -> prev = NULL;
            block -> prev = flist_pointer;
            block -> next = NULL;
            flist = flist_pointer;
            return;
        }  
        */

        /*
        if (block_size > flist_pointer_size)
        {
            flist_pointer -> next = block;
            flist_pointer -> prev = NULL;
            block -> prev = flist_pointer;
            block -> next = NULL;
            flist = flist_pointer;
            return;
        }
        */
        
        
        struct head *bflist_pointer = flist_pointer -> prev;
        block -> next = flist_pointer;
        block -> prev = bflist_pointer;
        flist_pointer -> prev = block;

        if (bflist_pointer != NULL)
        {
            bflist_pointer -> next = block;
        }
        else
        {
            flist = block;
        }
    }
}

/*
void sort()
{
    struct head *flist_pointer = flist;
    struct head *bflist_pointer = flist;

    if (flist_pointer -> next != NULL)
    {
        while (flist_pointer -> size < flist_pointer -> next -> size)
        {
            flist_pointer = flist_pointer -> next;
            bflist_pointer = flist_pointer;
        }

        if (flist_pointer -> next != NULL)
        {
            bli
        }
        
    }
}
*/

// split -> old
struct head *split(struct head *block, int size)
{
    int rsize = (block -> size) - size - HEAD;
    block -> size = rsize; 

    struct head *splt = after(block);
    splt -> bsize = rsize;
    splt -> bfree = TRUE;
    splt -> size = size;
    splt -> free = FALSE;

    struct head *aft = after(splt);
    aft -> bsize = rsize;

    detach(block);
    insert(block);

    return splt;
}

int adjust(int request)
{
    int size = ALIGN;
    if(request < size)
    {
        return size;
    }
    else
    {
        int remainder = request % ALIGN;
        int factor = request / ALIGN;
        if (remainder != 0)
        {
            size = ALIGN * (factor + 1);
        }
        else
        {
            size = ALIGN * factor;
        }
        return size;
    }
}

struct head *find_space(int size, struct head *block)
{
    if(block == NULL)
    {
        return NULL;
    }
    else
    {
        if(block -> size >= size)
        {
            if(block -> size >= size + ALIGN + HEAD)
            {
               return split(block, size);
            }
            else
            {
                detach(block);
                return block;
            }
        }
        else
        {
            find_space(size, block -> next);
        }
    }
}

struct head *find(int size)
{
    return find_space(size, flist);
}

struct head *merge(struct head *block)
{
    struct head *aft = after(block);

    if (block -> bfree && block != arena)
    {
        struct head *bblock = before(block);
        int total_size = bblock -> size + block -> size + HEAD;
        struct head *ablock = after(block);
        detach(block);
        bblock -> size = total_size;

        if (ablock -> size != 0)
        {
            ablock -> bsize = total_size;
        }

        block = merge(bblock);
    }

    if (aft -> free)
    {
        struct head *ablock = after(block);
        int total_size = block -> size + ablock -> size + HEAD;
        struct head *aablock = after(ablock);
        detach(ablock);
        block -> size = total_size;

        if (aablock != NULL)
        {
            aablock -> bsize = total_size;
        }

        block = merge(ablock);
    }

    return block;
}

void *dalloc(size_t request)
{
    if(request <= 0)
    {
        return NULL;
    }
    int size = adjust(request);
    struct head *taken = find(size);
    if(taken == NULL)
    {
        return NULL;
    }
    else
    {
        return HIDE(taken);
    }
}

void pfree(void *memory)
{
    if(memory != NULL)
    {
        struct head *block = MAGIC(memory);

        struct head *aft = flist;
        block -> free = TRUE;
        insert(block);
        // merge section
        //merge(block);
        // merge section done
    }
    return;
}

void another_data()
{
    int arena_counter = 0;
    struct head *arena_pointer = arena;
    while (arena_pointer -> size != 0)
    {
        int arena_free = arena_pointer -> free;
        int arena_size = arena_pointer -> size;
        int arena_bfree = arena_pointer -> bfree;
        int arena_bsize = arena_pointer -> bsize;
        printf("%d\n", arena_size);
        arena_pointer = after(arena_pointer);
        arena_counter++;
    }
}
