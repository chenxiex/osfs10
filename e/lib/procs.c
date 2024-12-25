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

PUBLIC int get_procs(struct sproc *proc_t)
{
    MESSAGE msg;
    msg.type = PROCS;
    msg.BUF = proc_t;
    send_recv(BOTH, TASK_MM, &msg);
    assert(msg.type == SYSCALL_RET);
    return msg.RETVAL;
}