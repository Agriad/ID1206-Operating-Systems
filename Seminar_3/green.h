#include <ucontext.h>

typedef struct green_t
{
    ucontext_t *context;
    void *(*fun)(void*);
    void *arg;
    struct green_t *next;
    struct green_t *join;
    void *retval;
    int zombie;
}

green_t;

typedef struct green_cond_t
{
    green_t *suspend_first;
    green_t *suspend_last;
}

green_cond_t;

typedef struct green_mutex_t
{
    volatile int taken;
    //handle the list
    green_t *mutex_first;
    green_t *mutex_last;
}

green_mutex_t;

int green_create(green_t *thread, void *(*fun)(void*), void *arg);
int green_yield();
int green_join(green_t *thread, void** val);

void green_cond_init(green_cond_t *thread);
void green_cond_wait(green_cond_t *thread);
void green_cond_signal(green_cond_t *thread);

int green_mutex_init(green_mutex_t *mutex);
int green_mutex_lock(green_mutex_t *mutex);
int green_mutex_unlock(green_mutex_t *mutex);