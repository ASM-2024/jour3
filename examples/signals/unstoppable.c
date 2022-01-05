#define _POSIX_C_SOURCE 3

#include <err.h>
#include <signal.h>
#include <stdio.h>

void handler(int signum)
{
    printf("I cannot be terminated with signal %d\n", signum);
}

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigemptyset(&sa.sa_mask) == -1)
    {
        errx(1, "Error in sigemptyset");
    }
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        errx(1, "Can't link handler to signal");
    }

    while (1)
        ;
    return 0;
}
