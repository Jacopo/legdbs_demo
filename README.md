Get this with `git clone --recursive https://github.com/Jacopo/legdbs_demo`

0. `sudo apt-get install linux-libc-dev linux-libc-dev:i386 && make checks`
1. `./vuln shell`
2. `make`  ( <-- Will "embed" [fix.c](https://github.com/Jacopo/legdbs_demo/blob/master/fix.c) in `vuln`)
3. `./vuln_fixed shell`

Enjoy :)


Why in the world would you do this?!?
=====================================

Sometimes you want to change something in how a binary runs, `LD_PRELOAD` doesn't cut it (e.g., static binary -- check out (preeny)[https://github.com/zardus/preeny] otherwise!), and you find patching with handwritten assembly annoying or unfit for the job.

Oh, and `ptrace()` and friends are not available, as [LegitBS](https://legitbs.net/) tends to prefer at the [DEFCON CTF finals](https://www.defcon.org/html/links/dc-ctf.html), or for other reasons.

**Examples**:

* Wrapping functions inside the main program (own crypto, for instance) to log what is going on / react to not-previously-seen states.
* Check the stack when a particularly suspicious function is called.
* Pretty much any case in which observing the program state gives useful info.

(Or if, god help us, everything is static and you need to "port" general CTF defenses.)


How would I use this?
=====================

A way to use it is:

0. given the static binary [vuln](https://github.com/Jacopo/legdbs_demo/blob/master/vuln), one would
1. find a function of interest (to patch it, log its parameters, etc.),
2. set a breakpoint there ([before_entry](https://github.com/Jacopo/legdbs_demo/blob/master/fix.c#L35)) and
3. write a function that examines the program state and takes some action ([fixit](https://github.com/Jacopo/legdbs_demo/blob/master/fix.c#L7)).

Writing and injecting [fix.c](https://github.com/Jacopo/legdbs_demo/blob/master/fix.c) using those tools is [almost too easy](https://github.com/Jacopo/legdbs_demo/blob/master/Makefile)!

But this _is_ experimental code written in a rush, so I make no warranties!

_NOTE_: You can do pretty much anything when a breakpoint is hit, but you cannot easily use the full glibc (global state, no linking, etc.) against a static binary.

However, I recycled code from an old project of mine to create (the illusion of) a having a mini libc-like library. This should make it all _much_ easier but it's obviously incomplete and probably has bugs. Again, no warranty :)

Most stuff is cross-platform, but only x86 is supported at the moment. The best course of action would be to adapt it to use, for instance, Chromium's [linux-syscall-support](https://chromium.googlesource.com/linux-syscall-support) to gain better support.


How would this even work?!?
===========================

It's in three, rather independent, pieces:

- [Code to create breakpoints, and call a function on hit](https://github.com/Jacopo/legdbs) -- a bit like an in-process gdb, if you will :)  It's decently cross-platform, just needs an opcode that causes a signal to be sent (and freedom to catch it).
- [A "fake" inline libc](https://github.com/Jacopo/inline-notlibc), which uses Linux's userland headers as its reference to make syscalls, and keeps no state at all. Inject and use anywhere.
- [A tool to inject code](https://github.com/Jacopo/elf_add_code) (compiled ELF, not just raw binary) at the program's entry point. That way one can set the breakpoints immediately.
