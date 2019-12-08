#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
//static green_t main_green = {&main_cntx, NULL, ... FALSE};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static green_t *running = &main_green;

static void init() __attribute__((constructor));

void init()
{
    getcontext(&main_cntx);
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
    ucontext_t *cntx = thread -> context;
    free(cntx -> uc_stack.ss_sp);
    free(cntx);
}

void *green_thread()
{
    green_t *this = running;

    // segmentation fault
    void *result = (*this -> fun)(this -> arg);

    // place waiting (joining) thread in ready queue
    // we use join as a a storage to see who is waiting for us
    if (this -> join != NULL)
    {
        ready_list_add(this -> join);
    }

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
    makecontext(cntx, green_thread, 0);

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

void green_cond_init(green_cond_t *condition)
{
    green_cond_t *new_green_cond_t;
    condition = new_green_cond_t;
}

void green_cond_wait(green_cond_t *condition)
{
    green_t *to_suspend = running;
    green_t *green_thread = condition -> suspend_last;

    if (green_thread == NULL)
    {
        condition -> suspend_first = to_suspend;
    }
    else
    {
        condition -> suspend_last -> next = to_suspend;
    }

    condition -> suspend_last = to_suspend;
    to_suspend -> next = NULL;
    green_yield();
}

void green_cond_signal(green_cond_t *condition)
{
    green_t *green_thread = condition -> suspend_first;

    if (green_thread != NULL)
    {
        if (green_thread -> next == NULL)
        {
            condition -> suspend_first = NULL;
            condition -> suspend_last = NULL;
        }
        else
        {
            condition -> suspend_first = green_thread -> next;
        }

        green_thread -> next = NULL;
        ready_list_add(green_thread);
    }

    /*
    if (green_thread -> next == NULL)
    {
        condition -> suspend_first = NULL;
        condition -> suspend_last = NULL;
    }
    else
    {
        condition -> suspend_first = green_thread -> next;
    }

    green_thread -> next = NULL;
    ready_list_add(green_thread);
    */
}

/*

A calls green_thread()
    B waits for A
    B takes A -> retval
A -> join = B

greenjoin(A)
    initialize A -> join = B

*/