#include "stdio.h"
#include "type.h"

const unsigned long long TIME_UNIT = 10000000;

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
    if (argc != 2)
    {
        printf("Invalid argument\n");
        return 1;
    }
    int time = stoi(argv[1]);
    if (time == -1)
    {
        printf("Invalid argument\n");
        return 1;
    }
    for (int i = 0; i < time; i++)
    {
        for (unsigned long long j = 0; j < TIME_UNIT; j++)
        {
            // do nothing
        }
    }
}