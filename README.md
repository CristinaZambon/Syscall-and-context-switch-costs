## Intro

In this homework assignment, the goal is to measure the costs associated with system calls and context switches in an operating system. Refer to Chapter 6, page 63 of Operating Systems: Three Easy Pieces.

## 1. Measurement of the cost of a system call
I measured the average time that elapsed among consecutive calls to read(). As suggested
from the book, my program performs a 0-byte reading from standard input. By including the
library “unistd.h”, I can use “0” as file descriptor of standard input.
In order to get the starting and ending time of the while loop, I re-use a code provided by
the GitHub of the book ostep: https://github.com/remzi-arpacidusseau/ostep-homework/
blob/561fe0fe0f83dafba54033abd5b50b876420415f/vm-beyondphys/mem.c.
In the function GetTime(), we use gettimeofday(). Its first argument refers to the struct
timeval t that we use in order to handle both seconds and microseconds. This is done by
tv_sec and tv_usec respectively that are part of timeval t. The second argument of
gettimeofday() is set to NULL because we don’t care about the time zone. gettimeofday()
returns 0 when succeeds, in this case the assert (rc==0) does nothing.
In my program, I print to screen the cost of the system call read() with the unit of
measurement of microseconds. In this way, I obtain better precision of the time interval.

## 2. Measurement of the cost of a context switch
The program takes 2 command-line arguments: the CPU number for the parent and the
CPU number for the child. If they are different, the parent one is chosen as default.
I force the children to use the same cpu, even if a child created via fork inherits its parent's
CPU affinity mask. My program is based on the idea of communication between two
children processes through two pipes.
The first child process A writes “ This is the message from A!” in the first pipe ( in pipe1[1]
where the index 1 is for writing). Then, before reading, child A waits for child B as B has to
write its message in the second pipe. This is shown by the first line of the output.
That means that process A is blocked, while process B is ready; process B can start writing
its message. After writing, B is blocked and A can finally read from the pipe2.
I iterate these read & write operations with a while loop in order to get the average cost of
context swtiching.
In order to measure time, I use the same function of the first program above.
We can also obtain the average cost in microseconds or nanoseconds by multiplying the
result by 1e6 or 1e9 respectively.
