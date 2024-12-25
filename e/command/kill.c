#include "stdio.h"
#include "type.h"

PRIVATE int stoi(char *str)
{
    int result = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return -1;
        }
        result = (result << 3) + (result << 1) + str[i] - '0';
    }
    return result;
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        int pid = stoi(argv[i]);
        if (pid == -1)
        {
            printf("Invalid pid: %s\n", argv[i]);
            continue;
        }
        int status = kill(pid);
        if (status == 0)
        {
            printf("process %d killed\n", pid);
        }else{
            printf("killing %d failed, status: %d \n", pid, status);
        }
    }
    return 0;
}