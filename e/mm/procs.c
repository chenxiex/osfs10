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
#include "keyboard.h"
#include "proto.h"

PUBLIC int do_procs()
{
    struct proc *p = proc_table;
    struct sproc mp[NR_TASKS + NR_PROCS];
    int src=mm_msg.source;
    void *buf=mm_msg.BUF;

    for (int i = 0; i < NR_TASKS + NR_PROCS; i++, p++)
    {
        strcpy(mp[i].name, p->name);
        mp[i].p_flags = p->p_flags;
        mp[i].exit_status = p->exit_status;
        mp[i].p_parent= p->p_parent;
        mp[i].priority= p->priority;
    }
    phys_copy(va2la(src, buf), va2la(TASK_MM, mp), sizeof(struct sproc) * (NR_TASKS + NR_PROCS));
    return 0;
}