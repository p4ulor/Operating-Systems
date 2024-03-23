        .section .rodata

data:   .byte  47, 101, 116,  99
        .byte  47, 111, 115,  45
        .byte 114, 101, 108, 101
        .byte  97, 115, 101,   0
        .byte  73,  83,  69,  76

        .text
        .globl _start
_start:
        movq $-100, %rdi # not relevant, value will be ignored by the function https://linux.die.net/man/2/openat#:~:text=If%20pathname%20is%20absolute%2C%20then%20dirfd%20is%20ignored
        leaq data(%rip), %rsi # leaq moves the reference of the data starting at 47
        xorq %rdx, %rdx # set rdx = 0 (O_RDONLY)
        movq $257, %rax # set intruction -> openat
        syscall # int fileDescriptor = openat(-100, data*, 0) (open and possibly create a file)
        # openat(int dirfd, const char *pathname, int flags);   

        movq %rax, %r15 # saves data in the form of a file-descriptor

        movq %r15, %rdi # set rdi = fileDescriptor
        xorq %rsi, %rsi # set rsi = 0 (offset)
        movq $2, %rdx # set rdx = 2 = SEEK_END https://support.sas.com/documentation/onlinedoc/ccompiler/doc700/html/lr1/z2031150.htm
        movq $8, %rax # set intruction -> lseek
        syscall # int dataSize = lseek(fileDescriptor, 0, SEEK_END)
        # lseek(int fd, off_t offset, int whence)

        movq %rax, %r14 # save dataSize

        xorq %rdi, %rdi # set rdi = 0
        movq %r14, %rsi # set rsi = dataSize
        movq $1, %rdx # set rdx = 1 (PROT_READ)
        movq $2, %r10 # set r10 = 2 (MAP_PRIVATE flag)
        movq %r15, %r8 # set r8 = fileDescriptor
        xorq %r9, %r9 # set r9 = 0 (offset)
        movq $9, %rax # set intruction -> mmap
        syscall # void* pointer = mmap(0, dataSize, 1, 2, fileDescriptor, 0)
        #      start of map addr, lenght of map, protection
        # mmap(void addr[.length], size_t length, int prot, int flags, int fd, off_t offset);

        movq $1, %rdi # set rdi = 1 (STDOUT)
        movq %rax, %rsi # rsi = pointer
        movq %r14, %rdx # rdx = dataSize
        movq $1, %rax # set intruction -> write
        syscall # ssize_t bytesWritten = write(1, pointer, dataSize)
        # write(int fd, const void buf[.count], size_t count);

        xorq %rdi, %rdi # rdi = 0
        movq $231, %rax # set intruction -> exit_group
        syscall # end process with return code 0

        .end
