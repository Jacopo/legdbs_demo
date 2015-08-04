all: vuln_fixed

HANDLER_CFLAGS = -std=gnu99 -nostdlib -Wl,--build-id=none -DINDEPENDENT -static
fix.o: fix.c legdbs/legdbs.c
	@echo "*** Making the injectable object file with your fix..."
	gcc -m32 -Wall $(HANDLER_CFLAGS) -Wl,-r -o $@ $^

vuln_fixed: fix.o vuln deps
	@echo "*** Injecting your object in the vuln binary..."
	./elf_add_code/add_code_32 --before-entry vuln fix.o > $@
	chmod a+x $@


deps:
	make -s -C elf_add_code

checks:
	make -C elf_add_code check
	make -C legdbs check

clean:
	make -C elf_add_code clean
	make -C legdbs clean
	rm -f fix.o vuln_fixed

.PHONY: all clean deps checks
