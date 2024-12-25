#include "stdio.h"
#include "type.h"
#define FREE_SLOT 0x20

const int PROC_NUM = 64;

int main()
{
    struct sproc sproc_table[PROC_NUM];
    for (int i = 0; i < PROC_NUM; i++)
    {
        sproc_table[i].p_flags = FREE_SLOT;
    }
    get_procs(sproc_table);
    for (int i = 0; i < PROC_NUM; i++)
    {
        if (sproc_table[i].p_flags != FREE_SLOT)
        {
            printf("pid:%d    name:%s\n", i, sproc_table[i].name);
        }
    }
}