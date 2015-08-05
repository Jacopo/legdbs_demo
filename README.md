0. `sudo apt-get install linux-libc-dev linux-libc-dev:i386 && git submodule sync && make checks`
1. `./vuln shell`
2. `make`  ( <-- Will "embed" [fix.c](fix.c) in `vuln`)
3. `./vuln_fixed shell`

Enjoy :)
