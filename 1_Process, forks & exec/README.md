# 1 - Process (& Threads)
- A thread is an independent flow of control that operates within the same address space as other independent flows of controls within a process
- Traditionally, thread and process characteristics are grouped into a single entity called a process. In other operating systems, threads are sometimes called lightweight processes, or the meaning of the word thread is sometimes slightly different
- Every "program" is associated with a process or more. And every thread has it's own thread or more. A process has more capabilities than a thread
- Every process has an ID (PID) which is a positive integer

# 2 - File descriptors
An unsigned integer used by a process to identify an open file. In a Linux Environment, it's OK to say that everything is basically a file. So this "file" can actually be an I/O device or network

By default every process has the file descriptors:
- 0 -> standard input
- 1 -> standard output
- 2 -> standard error

You can create more file descriptors when executing a program in a command line (via '<' or '>') or through the code the program runs

## 2.1 - Demo w/ CMD line
```c
$ echo Hello
Hello
$ echo Hello > hello.txt // redirects & writes the standard output (by default) coming from the program "echo" to a file
$ echo Hello 1> hello.txt // explicitely redirect standard output to a file
$ echo Hello 100> new.txt // Since the program "echo" doesn't use file desc w/ ID = 100, it will still print in standart output, but the file "new.txt" will be created
Hello
$ cat < hello.txt // Read from file. The '<' is optional actually
Hello
$ 
```

### Extra demo: File copy
`cat` will read hello.txt and what `cat` would otherwise output, will be redirected to a file
```c
$ cat < hello.txt > anotherhello.txt //The cmds will execute from left to right
```

## 2.2 - How to create new file descriptors
- dup() -> creates a file descriptor and returns it's identifying integer
- dup2(1, 88) -> creates a file descriptor and returns it's identifying integer (88). The 1st param is the number of the file descriptor to be replaced and the 2nd is the replacer

# 3 - Fork
- fork() duplicates the current process. The exact code & instruction (including the returned PID integer) of the fork() after the method call will be distinct for the parent process (the one that called the method) and the child process. The parent will have the returned PID of the child and the child will have 0 returned to it. -1 is returned to the process that called fork() if there's no memory or in other strange cases
- The current process can call `getpid()` to get it's PID
- A process that has a parent can call `getppid()` to get it's parent PID
- The address spaces & memory will be different
- The fork() duplicates the parent process, but duplicates only the calling thread; the child process is a single-threaded process. The calling thread of the parent process becomes the initial thread of the child process; it may not be the initial thread of the parent process. Thus, if the initial thread of the child process returns from its entry-point routine, the child process terminates
- The parent process can only end when the initial/main thread of the child terminates
- Again, note that the child will run the exact code that is left to run. To run different code, use `exec command`. Variables writes are done in different addresses.

# 4 - Exec command
- execlp()

# References
- https://www.ibm.com/docs/en/aix/7.1?topic=programming-understanding-threads-processes
- https://www.ibm.com/docs/en/aix/7.1?topic=volumes-using-file-descriptors
- https://www.ibm.com/docs/en/aix/7.1?topic=programming-process-duplication-termination
- https://stackoverflow.com/questions/985051/what-is-the-purpose-of-fork