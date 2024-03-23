
// Este ficheiro contém a implementação dos seguintes stubs para
// chamadas de sistema.

// argsFromCall - rdi   rsi   rdx   rcx   r8    r9
// argsToUse - rdi   rsi   rdx   r10   r8    r9

# pid_t   isel_getpid();
# void    isel_exit(int status);
# int     isel_open(const char *pathname, int flags, ...);
# off_t   isel_lseek(int fd, off_t offset, int whence);
# int     isel_close(int fd);
# void *  isel_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

	.text
	
	.global isel_getpid
	.global isel_exit
	.global isel_open
	.global isel_lseek
	.global isel_close
	.global isel_mmap

isel_getpid:
    mov $0x27, %rax
	syscall
    ret

isel_exit:
    // exit only has one argument passed through rdi, wich is one the right address
    mov $0x3c, %rax
	syscall
    ret

isel_open:
    mov $0x02, %rax
	syscall
    ret

isel_lseek:
    mov $0x08, %rax
	syscall
    ret

isel_close:
    mov $0x03, %rax
	syscall
    ret

isel_mmap:
    mov %rcx, %r10
    mov $0x09, %rax
	syscall
    ret

	.end

