#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include <signal.h>
#include <sys/time.h>
#include "green.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096
#define PERIOD 100

static ucontext_t main_cntx = {0};
//static green_t main_green = {&main_cntx, NULL, ... FALSE};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static green_t *running = &main_green;

static sigset_t block;

void timer_handler(int);

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

void green_thread()
{
    green_t *this = running;

    // segmentation fault
    void *result = (*this -> fun)(this -> arg);

    // place waiting (joining) thread in ready queue
    // we use join as a a storage to see who is waiting for us
    if (this -> join != NULL)
    {
        sigprocmask(SIG_BLOCK, &block, NULL);
        ready_list_add(this -> join);
        sigprocmask(SIG_UNBLOCK, &block, NULL);
    }

    // save result of execution
    this -> retval = result;

    // we're a zombie
    this -> zombie = TRUE;

    // find the next thread to run
    sigprocmask(SIG_BLOCK, &block, NULL);
    green_t *next  = ready_list_remove();
    sigprocmask(SIG_UNBLOCK, &block, NULL);

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
    sigprocmask(SIG_BLOCK, &block, NULL);
    ready_list_add(new);
    sigprocmask(SIG_UNBLOCK, &block, NULL);

    return 0;
}

int green_yield()
{
    sigprocmask(SIG_BLOCK, &block, NULL);
    green_t *susp = running;
    // add susp to ready queue
    ready_list_add(susp);

    // select the next thread for execution
    green_t *next = ready_list_remove();

    running = next;
    swapcontext(susp -> context, next -> context);
    sigprocmask(SIG_UNBLOCK, &block, NULL);
    return 0;
}

int green_join(green_t *thread, void **res)
{
    if(!thread -> zombie)
    {
        green_t *susp = running;
        // add as joining thread
        thread -> join = susp;

        sigprocmask(SIG_BLOCK, &block, NULL);
        // select the next thread for execution
        green_t *next = ready_list_remove();
        sigprocmask(SIG_UNBLOCK, &block, NULL);

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
    sigprocmask(SIG_BLOCK, &block, NULL);
    running = ready_list_remove();
    swapcontext(to_suspend -> context, running -> context);
    sigprocmask(SIG_UNBLOCK, &block, NULL);
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
        sigprocmask(SIG_BLOCK, &block, NULL);
        ready_list_add(green_thread);
        sigprocmask(SIG_UNBLOCK, &block, NULL);
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

void timer_handler(int sig)
{
    green_t *susp = running;

    // add the running to the ready queue
    ready_list_add(susp);

    // find the next thread for execution
    green_t *next = ready_list_remove();

    running = next;
    swapcontext(susp -> context, next -> context);
}

static void init() __attribute__((constructor));

void init()
{
    getcontext(&main_cntx);
    sigemptyset(&block);
    sigaddset(&block, SIGVTALRM);

    struct sigaction act = {0};
    struct timeval interval;
    struct itimerval period;

    act.sa_handler = timer_handler;
    assert(sigaction(SIGVTALRM, &act, NULL) == 0);

    interval.tv_sec = 0;
    interval.tv_usec = PERIOD;
    period.it_interval = interval;
    period.it_value = interval;
    setitimer(ITIMER_VIRTUAL, &period, NULL);
}

int green_mutex_init(green_mutex_t *mutex)
{
    mutex -> taken = FALSE;
    // initialize fields
    mutex -> mutex_first = NULL;
    mutex -> mutex_last = NULL;
}

int green_mutex_lock(green_mutex_t *mutex)
{
    // block timer interrupt
    sigprocmask(SIG_BLOCK, &block, NULL);

    green_t *susp = running;
    green_t *mutex_thread = mutex -> mutex_last;
    if (mutex -> taken)
    {
        // suspend the running thread
        if (mutex_thread == NULL)
        {
            mutex -> mutex_first = susp;
        }
        else
        {
            mutex -> mutex_last -> next = susp;
        }

        mutex -> mutex_last = susp;
        susp -> next = NULL;
        // find the next thread
        green_t *next = ready_list_remove();
        running = next;
        swapcontext(susp -> context, next -> context);
    }
    else
    {
        // take the lock
        mutex -> taken = TRUE;
    }
    // unblock
    sigprocmask(SIG_UNBLOCK, &block, NULL);

    return 0;
}

int green_mutex_unlock(green_mutex_t *mutex)
{
    // block timer interrupt
    sigprocmask(SIG_BLOCK, &block, NULL);

    //if (mutex -> susp != NULL)
    green_t *mutex_thread = mutex -> mutex_first;
    if (mutex_thread != NULL)
    {
        // move suspended threads to ready queue
        if (mutex_thread -> next == NULL)
        {
            mutex -> mutex_first = NULL;
            mutex -> mutex_last = NULL;
        }
        else
        {
            mutex -> mutex_first = mutex_thread -> next;
        }

        mutex_thread -> next = NULL;
        ready_list_add(mutex_thread);
    }
    else
    {
        // release lock
        mutex -> taken = FALSE;
    }
    // unlock
    sigprocmask(SIG_UNBLOCK, &block, NULL);

    return 0;
}

/*

A calls green_thread()
    B waits for A
    B takes A -> retval
A -> join = B

greenjoin(A)
    initialize A -> join = B

*/