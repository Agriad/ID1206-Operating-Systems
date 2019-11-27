#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>

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
    //return (struct head*)();
    return (struct head*)((char *)(HEAD + block -> size));
}

struct head *before(struct head *block)
{
    //return (struct head*)();
    return (struct head*)((char *) (HEAD + block -> bsize));
}

struct head *split(struct head *block, int size)
{
    /*
    int rsize =
    block -> size = 

    struct head *split = 
    splt -> bsize = 
    splt -> bfre = 
    splt -> size = 
    splt -> free = 

    struct head *aft = 
    aft -> bsize = 

    return splt;
    */

    int rsize = size;
    block -> size = (block -> size) - rsize - HEAD; 

    struct head *splt = malloc(HEAD);
    splt -> bsize = block -> size;
    splt -> bfree = TRUE;
    splt -> size = rsize;
    splt -> free = FALSE;

    struct head *aft = block;
    aft -> bsize = block -> bsize;

    return splt;
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
    uint size = ARENA - 2 * HEAD;

    /*
    new -> bfree =;
    new -> bsize =;
    new -> free =;
    new -> size =;
    */

    new -> bfree = FALSE;
    new -> bsize = NULL;
    new -> free = TRUE;
    new -> size = size - HEAD;

    struct head *sentinel = after(new);

    /* only touch the status fields */
    /*
    sentinel -> bfree =;
    sentinel -> bsize =;
    sentinel -> free =;
    sentinel -> size =;
    */

    sentinel -> bfree = FALSE;
    sentinel -> bsize = NULL;
    sentinel -> free = FALSE;
    sentinel -> size = NULL;

    /* this is the only arena we have */
    arena = (struct head*) new;

    return new;
};

// srcut head *flist;
struct head *flist;

void detach(struct head *block)
{
    /*
    if(block -> next != NULL)
    {}
    if (block -> prev != NULL)
    {}
    else
    {}
    */

    if(block -> next != NULL)
    {
        struct head *next_head = block -> next;
        next_head -> prev = block -> prev;       
    }
    if (block -> prev != NULL)
    {
        struct head *prev_head = block -> prev;
        prev_head -> next = block -> next;
    }
    else
    {
        flist == NULL;
    }
}

void insert(struct head *block)
{
    /*
    block -> next =;
    block -> prev =;
    if(flist != NULL)
    {}
    flist =;
    */

    block -> next = flist -> next;
    block -> prev = NULL;
    if(flist != NULL)
    {
        flist -> prev = block;
    }
    flist = block;
}

int adjust(int request)
{
    return HEAD + request;
}

struct head find_space(int size, struct head *block)
{
    if(block == NULL)
    {
        return NULL;
    }
    else
    {
        if (block -> size >= size)
        {
            if (size < 8)
            {
                return split(block, 8);
            }
            else
            {
                return split(block, size);
            }   
        }
        else
        {
            find_space(size, block -> next);
        }
           
    }
}

struct head find(int size)
{
    return find_space(size, flist);
}

void *dalloc(size_t request)
{
    /*
    if(request <= 0)
    {
        return;
    }
    int size = adjust(request);
    struct head *taken = find(size);
    if(taken == NULL)
    {
        return NULL;
    }
    else
    {
        return;
    }
    */

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
        return taken;
    }
}

void pfree(void *memory)
{
    if(memory != NULL)
    {
        /*
        struct head *block =;

        struct head *aft =;
        block -> free =;
        aft -> bfree =;
        */

        struct head *block = memory;

        struct head *aft = flist;
        block -> free = TRUE;
        aft -> bfree = TRUE;

        detach(block);
        insert(block);
    }
    return;
}