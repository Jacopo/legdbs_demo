/* A statically-linked vulnerable program that doesn't like ptrace */

#include <sys/ptrace.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    fprintf(stderr, "Hi! I'm most definitely not a dangerous program!\n");
//    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1)
//        errx(10, "Hey, don't ptrace me! That's rude!");

    if (argc != 2)
        errx(1, "Tell me what you want! Like, '%s muffin' will give you a muffin.", argv[0]);

    if (strcmp(argv[1], "shell") == 0) {
        fprintf(stderr, "Want a shell? Here it is!\n");
        system("/bin/bash");
    } else fprintf(stderr, "Look how nice I am! Here's a %s :)\n", argv[1]);

    return 0;
}
