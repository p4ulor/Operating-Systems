# 1 - Pipe
- A system call that creates a unidirectional communication link between two file descriptors. The pipe system call is called with a pointer to an array of two integers. The 1st int will be the one used for input by default
- This is useful so that different processes can send data between each other

## Example
- Create file descriptors 3 & 4
- Fork process
- Child also has file descp 3 & 4
- To make the Child communicate to the Parent, close FD=4 in the Parent and close the FD=3 in the Child
- So the Child now outputs to FD=4 and the Parent can obtain inputs by reading from FD=3

# 2 - Signals
- A software generated interrupt that is sent to a process by the OS because of when user press CTRL + C or another process tell something to this process. There are fix set of signals that can be sent to a process. signal are identified by integers.