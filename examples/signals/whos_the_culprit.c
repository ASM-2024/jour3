#define _POSIX_C_SOURCE 199309L

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void segv_handler(int sig, siginfo_t *siginfo, void *context)
{
    printf("Receive signal %d\n", sig);
    printf("  address related to fault: %p\n", siginfo->si_addr);
    printf("  context address: %p\n", context);
    _exit(sig);
}

int main()
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_sigaction = &segv_handler;
    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGSEGV, &action, NULL) < 0)
        err(1, "sigaction");

    int *p = NULL;
    *p = 42;
    return 0;
}
