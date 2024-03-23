**In the source file cw2/ex2/arm64/prog.s, replace ? and ?? with adequate numbers, such that it becomes a working ARM64 version of cw2/ex2/x86-64/prog.s.**

___

Table used:
- https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#arm64-64_bit

I have an Intel CPU w/ the x86-64 architecture. Commands used:
- make
- qemu-aarch64 ./prog

The output is successfully the same

## Calling conventions for arm-64
- return -> x0
- arg0 -> x0
- arg1 -> x1
- arg2 -> x2
- arg3 -> x3
- arg4 -> x4
- arg5 -> x5

## Diff between x86-64 strace and arm64 strace
### x86-64 - $ qemu-x86_64 -strace ./prog > /dev/null
```
425521 openat(AT_FDCWD,"/etc/os-release",O_RDONLY) = 3
425521 lseek(3,0,SEEK_END) = 386
425521 mmap(NULL,386,PROT_READ,MAP_PRIVATE,3,0) = 0x0000004000803000
425521 write(1,0x803000,386) = 386
425521 exit_group(0)
```

### arm64 - $ qemu-aarch64 -strace ./prog > /dev/null
```
423146 openat(AT_FDCWD,"/etc/os-release",O_RDONLY) = 3
423146 lseek(3,0,SEEK_END) = 386
423146 mmap(NULL,386,PROT_READ,MAP_PRIVATE,3,0) = 0x0000005500802000
423146 write(1,0x802000,386) = 386
423146 exit_group(0)
```