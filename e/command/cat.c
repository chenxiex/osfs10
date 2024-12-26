#include "stdio.h"

const int BUF_SIZE = 1024;

int main(int argc, char *argv[])
{
    char buf[BUF_SIZE];
    int fd;
    struct stat s;
    if (argc == 1)
    {
        printf("Usage: cat filename\n");
        return 1;
    }
    if (stat(argv[1], &s) != 0)
    {
        printf("Cannot open file\n");
        return 1;
    }
    int fsize = s.st_size;
    if ((fd = open(argv[1], O_RDWR)) == -1)
    {
        printf("Cannot open file\n");
        return 1;
    }
    while (fsize > 0)
    {
        int read_cnt = fsize<BUF_SIZE?fsize:BUF_SIZE;
        read(fd, buf, read_cnt);
        fsize -= read_cnt;
        for (int i = 0; i < read_cnt; i++)
        {
            printf("%c", buf[i]);
        }
    }
    printf("\n");
    return 0;
}