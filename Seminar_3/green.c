#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
//static green_t main_green = {&main_cntx, NULL, ... FALSE};
static green_t main_green = {&main_cntx, NULL, FALSE};

static green_t *running = &main_green;

static void init() __attribute__((constructor));

void init()
{
    getcontext(&main_green);
}

green_t *ready_first;
green_t *ready_last;

// add context to end of ready queue
void ready_list_add(green_t *context)
{
    if (ready_first == NULL)
    {
        ready_first = context;
        ready_last = context;
    }
    else
    {
        ready_last -> next = context;
        ready_last = context;
    }
}

// remove context from the fornt of queue
green_t *ready_list_remove()
{
    if (ready_first == NULL)
    {
        return NULL;
    }
    else
    {
        if (ready_first -> next == NULL)
        {
            green_t *detached = ready_first;
            ready_first = NULL;
            ready_last = NULL;
            return detached;
        }
        else
        {
            green_t *detached = ready_first;
            ready_first = ready_first -> next;
            detached -> next = NULL;
            return detached;
        }
    }
}

// closes the memory that the thread is using as it finished it's work
void complete_thread(green_t *thread)
{
    
}

void *green_thread()
{
    green_t *this = running;

    void *result = (*this -> fun)(this -> arg);

    // place waiting (joining) thread in ready queue
    // we use join as a a storage to see who is waiting for us
    ready_list_add(this -> join);

    // save result of execution
    this -> retval = result;

    // we're a zombie
    this -> zombie = TRUE;

    // find the next thread to run
    green_t *next  = ready_list_remove();

    running = next;
    setcontext(next -> context);
}

int green_create(green_t *new, void *(*fun)(void*), void *arg)
{
    ucontext_t *cntx = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext(cntx);

    void *stack = malloc(STACK_SIZE);

    cntx -> uc_stack.ss_sp = stack;
    cntx -> uc_stack.ss_size = STACK_SIZE;
    makecontext(cntx, green_thread(), 0);

    new -> context = cntx;
    new -> fun = fun;
    new -> arg = arg;
    new -> next = NULL;
    new -> join = NULL;
    new -> retval = NULL;
    new -> zombie = FALSE;

    // add new to the ready queue
    ready_list_add(new);

    return 0;
}

int green_yield()
{
    green_t *susp = running;
    // add susp to ready queue
    ready_list_add(susp);

    // select the next thread for execution
    green_t *next = ready_list_remove();

    running = next;
    swapcontext(susp -> context, next -> context);
    return 0;
}

int green_join(green_t *thread, void **res)
{
    if(!thread -> zombie)
    {
        green_t *susp = running;
        // add as joining thread
        thread -> join = susp;

        // select the next thread for execution
        green_t *next = ready_list_remove();

        running = next;
        swapcontext(susp -> context, next -> context);
    }

    // collect result
    res = thread -> retval;

    // free context
    complete_thread(thread);

    return 0;
}