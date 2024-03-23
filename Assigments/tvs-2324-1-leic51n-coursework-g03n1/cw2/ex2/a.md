### Exercise 2

**a) List and explain the sequence of calls performed by the program in x86-64/prog.s**

----

I have an Intel CPU w/ the x86-64 architecture. Commands used:

## 1 - $ ./prog
Prints the contents of this [file](/etc/os-release):
```
PRETTY_NAME="Ubuntu 22.04.3 LTS"
NAME="Ubuntu"
VERSION_ID="22.04"
VERSION="22.04.3 LTS (Jammy Jellyfish)"
VERSION_CODENAME=jammy
ID=ubuntu
ID_LIKE=debian
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
UBUNTU_CODENAME=jammy
```

## 2 - $ strace ./prog > /dev/null
```
execve("./prog", ["./prog"], 0x7ffdc4570170 /* 97 vars */) = 0
openat(AT_FDCWD, "/etc/os-release", O_RDONLY) = 3
lseek(3, 0, SEEK_END)                   = 386
mmap(NULL, 386, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f1bc370d000
write(1, "PRETTY_NAME=\"Ubuntu 22.04.3 LTS\""..., 386) = 386
exit_group(0)                           = ?
+++ exited with 0 +++
```

## Calling conventions for x86-64
- return -> rax
- arg0 -> rdi
- arg1 -> rsi
- arg2 -> rdx
- arg3 -> r10
- arg4 -> r8
- arg5 -> r9

## Explanation of the assembly
- Comments were added in [prog.s](./x86-64/prog.s) file to companion the explanation
1. **int fileDescriptor = openat(-100, data\*, 0)** ➡️ A file descriptor is open upon the .data section
2. **int dataSize = lseek(fileDescriptor, 0, SEEK_END)** ➡️ Calculates the length of the file descriptor
3. **void\* pointer = mmap(0, dataSize, 1, 2, fileDescriptor, 0)** ➡️ Will map the file pointed by fileDescriptor into the address space (to the memory of the process). Using protection = PROT_READ, and using flag = MAP_PRIVATE, and offset = 0
4. **ssize_t bytesWritten = write(1, pointer, dataSize)** ➡️ print to stdout the contents of the pointer containing the contents of the file
5. **exit_group** ➡️ Exit process w/ return code 0
