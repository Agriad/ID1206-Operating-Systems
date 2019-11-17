#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid = fork();

    if(pid == 0)
    {
        execlp("ls", "ls", NULL);
        printf("This will only happen if exec fails\n");
    }
    else
    {
        wait(NULL);
        printf("We're done\n");
    }
 
    return 0;

    /*
    int pid = fork();

    if(pid == 0)
    {
        int child = getpid();
        printf("child: session %d\n", getsid(child));
    }
    else
    {
        int parent = getpid();
        printf("parent: session %d\n", getsid(parent));
    }
 
    return 0;
    */

    /*
    int pid = fork();

    if(pid == 0)
    {
        int child = getpid();
        printf("I'm the child %d, in group %d\n", child, getpgid(child));
    }
    else
    {
        int parent = getpid();
        printf("I'm the parent %d, in group %d\n", parent, getpgid(parent));
        wait(NULL);
    }
 
    return 0;
    */

    /*
    int pid = fork();

    if(pid == 0)
    {
        printf("I'm the child %d, with parent %d\n", getpid(), getppid());
    }
    else
    {
        printf("I'm the parent %d, with parent %d\n", getpid(), getppid());
        wait(NULL);
    }
 
    return 0;
    */

    /*
    int pid;
    int x = 123;
    pid = fork();

    if(pid == 0)
    {
        printf("child: x is %d and the address is 0x%p\n", x, &x);
        x = 42;
        sleep(1);
        printf("child: x is %d and the address is 0x%p\n", x, &x);
    }
    else
    {
        printf("mother: x is %d and the address is 0x%p\n", x, &x);
        x = 13;
        sleep(1);
        printf("mother: x is %d and the address is 0x%p\n", x, &x);
        wait(NULL);
    }
 
    return 0;
    */

    /*
    int pid = fork();

    if(pid == 0)
    {
        return 42;
    }
    else
    {
        int res;
        wait(&res);
        printf("The result was %d\n", WEXITSTATUS(res));
    }
 
    return 0;
    */

    /*
    int pid = fork();

    if(pid == 0)
    {
        printf("I'm the child %d\n", getpid());
        sleep(1);
    }
    else
    {
        printf("My child is called %d\n", pid);
        wait(NULL);
        printf("My child has terminated \n");
    }
    printf("This is the end (%d)\n", getpid());
    return 0;
    */

    /*
    int pid = fork();

    if(pid == 0)
    {
        printf("I'm the child %d\n", getpid());
    }
    else
    {
        printf("My child is called %d\n", pid);
    }
    printf("That's it %d\n", getpid());
    return 0;
    */

    /*
    int pid = fork();

    printf("pid = %d\n", pid);
    return 0;
    */
}