#include "stdio.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("touch: missing file operand\n");
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        int fd = open(argv[i], O_CREAT);
        if (fd == -1)
        {
            printf("touch: cannot touch \'%s\'\n", argv[i]);
            return 1;
        }
        else
        {
            close(fd);
        }
    }
    return 0;
}