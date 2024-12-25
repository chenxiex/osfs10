#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

PUBLIC int kill(int pid)
{
    if (pid >= NR_TASKS + NR_PROCS)
    {
        return -1;
    }
    MESSAGE msg;
    msg.type = KILL;
    msg.PID = pid;

    send_recv(BOTH, TASK_MM, &msg);
    if (msg.RETVAL==-1)
    {
        return -1;
    }
    assert(msg.type == SYSCALL_RET);
    return 0;
}