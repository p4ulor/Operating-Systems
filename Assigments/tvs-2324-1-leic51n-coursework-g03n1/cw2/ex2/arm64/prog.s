        .section .rodata

data:   .byte  47, 101, 116,  99
        .byte  47, 111, 115,  45
        .byte 114, 101, 108, 101
        .byte  97, 115, 101,   0
        .byte  73,  83,  69,  76

        .text
        .globl _start
_start:
        mov x0, #-100
        ldr x1, =data
        mov x2, #0
        # openat
        mov w8, #56
        # int fileDescriptor = openat(int dirfd, const char *pathname, int flags);
        svc #0

        # save fileDescriptor
        mov x21, x0

        mov x2, #2
        mov x1, #0
        mov x0, x21
        # lseek
        mov w8, #62
        # int dataSize = lseek(fileDescriptor, 0, SEEK_END)
        svc #0

        # save dataSize
        mov x20, x0

        mov x0, #0
        # (offset)
        mov x5, #0
        # PROT_READ
        mov x2, #1
        # MAP_PRIVATE flag
        mov x3, #2
        # dataSize
        mov x1, x20
        # fileDescriptor
        mov x4, x21
        # mmap, why the "?" ? https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/syscalls.md#aarch64_222
        mov w8, #222
        # void* pointer = mmap(0, dataSize, 1, 2, fileDescriptor, 0)
        svc #0

        # save pointer
        mov x19, x0

        # pointer
        mov x1, x19
        # dataSize
        mov x2, x20
        # (STDOUT)
        mov x0, #1
        # write
        mov w8, #64
        # ssize_t bytesWritten = write(1, pointer, dataSize)
        svc #0

        mov x0, #0
        # exit_group
        mov w8, #94
        svc #0

        .end
