**c) [OPTIONAL] In a text file, cw2/ex2/c.md or cw2/ex2/c.adoc, write a short text explaining what is QEMU User Emulation doing and, in particular, why is it relevant that it has a system call translator, as mentioned in the documentation: https://www.qemu.org/docs/master/user/main.html**
___

The emulator translates for each instruction, per example, the endianness of values, the order of the instructions, the name of the instructions, redirects and translates signals between the host & targets and handles 32/64-bit mismatches between the hosts & targets. Also, running `strace` on qemu shows that it uses a lot of `mmap` and `rt_sigaction` operations for it to work.

For handling the proper functionality threads, they run on a virtual CPU

So the goal of the system call translator, it to translate the encodings and arguments of CPU operations to the architecture of the user's machine CPU, before they are arrive there. Without the need the fully simulate the user machine's CPU