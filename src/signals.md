---
title: "Signals"
author: sergio.moubayed
logo: https://avatars.githubusercontent.com/u/96961500?s=200&v=4
---

# Signals
## Reminder
Since the memory used by two processes is distinct, we need mechanisms such as IPCs (Inter Process Communication) to enable such interactions. Signals are a a form of IPC that enable us to communicate between two processes.

To communicate, you need to identify the process through its PID (Process ID). The signal can then be sent by providing this same PID to "communicate" with this process.

You for sure came across some usages of signals without even realizing it. The [kernel](https://en.wikipedia.org/wiki/Kernel_(operating_system)) can use them to make sure you aren't doing anything wrong in your process. That's why most signal just kill your process.

## List signals
To list all signals, there is a command you could use for your system:
```bash
$ kill -l
```
The expected output (depending on your operating system) should look like the following:
```
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
63) SIGRTMAX-1	64) SIGRTMAX
```

### Example
For example, when you try to access a restricted memory address, the kernel doesn't like it and sends a signal to stop you. The name of the signal should not surprise you at all: ``SIGSEGV``. This should (if you went through S3)  trigger your PTSD.

## Tracing signals
To trace signals sent to a process we're going to use ``strace(1)`` (check the man page for more info).
```bash
$ strace -e 'trace=!all' [CMD]
```
You can have fun trying to see the different possible signals that you could get the kernel to send to your process.
### Exercise
Here are a few small C codes that should not work, try to figure out which signal is called by the kernel to kill them.
(For the sake of the exercise, do NOT compile with the usual flags)
- What the malloc?!
```c
#include <err.h>
#include <stdlib.h>

void *xmalloc(unsigned size)
{
    void *res = malloc(size);
    if (!res)
        errx(1, "Not enough memory!");
    return res;
}

int main(void)
{
    size_t i = 1;
    void *tmp = NULL;

    while (i >= 0)
    {
        tmp = xmalloc(i);
        i = i << 1;
    }

    return 0;
}
```
- Do I even exist?
```c
#include <stdio.h>

int main(void)
{
    int *answer = 42;
    printf("%d\n", *answer);

    return 0;
}
```
- One pie, Zero friends
```c
#include <stdio.h>

int is_prime(int n)
{
    for (int i = 0; i < n; i++)
        if (i != 1 && n % i == 0)
            return 0;
    return 1;
}

int main(void)
{
    for (int i = 2; i < 100; i++)
        if (is_prime(i))
            printf("%d\n", i);
    return 0;
}
```

## Catching signals
So C provides multiple ways to catch signals and change the original behavior of the process.
The most basic way is using ``signal(2)``:
```c
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```
There is a better alternative that we will favor: ``sigaction(2)``
```c
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```
### Exercise
The following exercises will only require you to play/modify with the following code
```c
#define _POSIX_C_SOURCE 200809l

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
        errx(1, "Error in sigemptyset");

    if (sigaction(SIGINT, &sa, NULL) == -1)
        errx(1, "Can't link handler to signal");

    while (1);

    return 0;
}
```
Ctrl-C usually sends a ``SIGINT`` (interrupt) signal to the program to stop it, we could intercept it and change its behavior to not stop the program.

Try to modify the signal intercepted and set it to ``SIGKILL`` and see what happens.
## Sending Signals
As mentioned prior, you only need a PID to send a signal to a process .

* On a terminal
```bash
$ kill -s [SIGNAL] [PID]
```
* In C use ``kill(2)``
```c
int kill(pid_t pid, int sig);
```
### Exercise
In this exercise, we'll ask you to play a small game.

First install the following [binary file](game), then make it executable through the following command:
```bash
chmod +x game
```
Then you can start the game by running the file like any other executable:
```bash
./game
```
The program will first start by displaying its own PID, it will then wait for you to send a specific signal through a terminal in less than the time displayed. When you go through 100 rounds, there is a small surprise at the end.

We do **not** expect you to go through the whole game **yet**. You can just have fun playing and discovering new signals.

#### Little spoiler for D4-workshop

You will have to write C code to "cheat" at this game, so you're basically going to automatize the game. To make it easier, only the signal names separated by ``\n ``  will be sent to ``stdout``. All the extra information will be written on ``stderr`` so that it could be ignored when you try to read from stdout.

Here are some commands to visualize what's happening:
```bash
# Redirecting stdout to /dev/null (removing stdout and keeping stderr)
./game > /dev/null
```
```bash
# Redirecting stderr to /dev/null (removing stdout and keeping stderr)
./game 2> /dev/null
```
