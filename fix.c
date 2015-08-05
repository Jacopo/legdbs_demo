#include "legdbs/legdbs.h"
#include "inline-notlibc/inline-notlibc.h"

#define PRINT(args...) fprintf(stderr, args)
//#define PRINT(args...)

void fixit(int sig, siginfo_t *info, struct ucontext *ctx)
{
    PRINT("[ ] Breakpoint hit at main(): %p\n", info->si_addr);

    uint32_t *esp = (uint32_t*) ctx->uc_mcontext.esp;
    uint32_t caller = esp[0];
    int argc = esp[1];
    char** argv = (char**) esp[2];

    PRINT("[ ] The call was main(argc = %d, argv = %p, ...)\n", argc, argv);

    if (argc > 1 && strcmp(argv[1], "shell") == 0) {
        PRINT("[X] INTERCEPTED!!!!\n");
        strcpy(argv[1], "SHELF"); // Sneaky change :)
    }

    // Or exit_group() / log / react to not-previously-seen states / do whatever you want

    // Example of logging
    int fd = open("/tmp/calls.log", O_CREAT|O_WRONLY|O_APPEND, 0666); VS(fd);
    write(fd, (uint8_t*) &caller, 4);
    { uint8_t c = ' '; write(fd, &c, 1); }
    if (argc > 1) // (logging post-change)
        write(fd, argv[1], strlen(argv[1]));
    { uint8_t c = '\n'; write(fd, &c, 1); }
    close(fd);
}

void before_entry()
{
    uintptr_t b = 0x08048BACu;  // This is the program's main()
    uintptr_t postbreak = b+4;  // This you can get using the script:
        // ./legdbs/print_set_breakpoint_line.py vuln main 2
        //                                            ^    ^
        //                                         addr    sizeof(BRK_OPCODE)

    PRINT("[ ] Adding a breakpoint at %p (service one at +%d = %p)\n",
            (void*) b, postbreak-b, (void*) postbreak);

    install_signal_handler();
    set_breakpoint(b, postbreak, fixit);
}
