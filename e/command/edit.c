#include "stdio.h"
#include "type.h"
#include "string.h"

const int BUF_SIZE = 1024;

struct stat s;
char *filename;

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

PRIVATE int enlarge_file(int size)
{
    int fd = open(filename, O_RDWR);
    if (lseek(fd, s.st_size, SEEK_SET) == -1)
    {
        printf("Failed to enlarge file\n");
        return 1;
    }

    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));

    while (size > 0)
    {
        int write_cnt = size < BUF_SIZE ? size : BUF_SIZE;
        if (write(fd, buf, write_cnt) == -1)
        {
            printf("Failed to enlarge file\n");
            return 1;
        }
        size -= write_cnt;
    }

    stat(filename, &s);
    close(fd);
    return 0;
}

PRIVATE int substitute(int pointer, char *content)
{
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        printf("Failed to open file\n");
        return 1;
    }

    if (pointer > s.st_size)
    {
        if (enlarge_file(pointer - s.st_size) != 0)
        {
            return 1;
        }
    }

    if (lseek(fd, pointer, SEEK_SET) == -1)
    {
        printf("Lseek failed\n");
        return 1;
    }
    if (write(fd, content, strlen(content) * sizeof(char)) == -1)
    {
        printf("Write failed\n");
        return 1;
    }

    stat(filename, &s);
    close(fd);
    return 0;
}

PRIVATE int insert(int pointer, char *content)
{
    int fd = open(filename, O_RDWR);
    int content_len = strlen(content);
    int org_size = s.st_size;
    if (fd == -1)
    {
        printf("Failed to open file\n");
        return 1;
    }

    if (enlarge_file(content_len) != 0)
    {
        return 1;
    }
    char charbuf[1];
    for (int i = org_size - 1; i >= pointer; i--)
    {
        if (lseek(fd, i, SEEK_SET) == -1)
        {
            printf("Lseek failed\n");
            return 1;
        }
        if (read(fd, charbuf, 1 * sizeof(char)) == -1)
        {
            printf("Read failed\n");
            return 1;
        }
        if (lseek(fd, i + content_len, SEEK_SET) == -1)
        {
            printf("Lseek failed\n");
            return 1;
        }
        if (write(fd, charbuf, 1 * sizeof(char)) == -1)
        {
            printf("Write failed\n");
            return 1;
        }
    }

    if (lseek(fd, pointer, SEEK_SET) == -1)
    {
        printf("Lseek failed\n");
        return 1;
    }
    if (write(fd, content, content_len * sizeof(char)) == -1)
    {
        printf("Write failed\n");
        return 1;
    }
 
    stat(filename, &s);
    close(fd);
    return 0;
}

PRIVATE int delete(int pointer, int length)
{
    int fd=open(filename,O_RDWR);
    if (fd == -1 || pointer >= s.st_size)
    {
        return 1;
    }
    if (pointer + length > s.st_size)
    {
        length = s.st_size - pointer;
    }

    char buf[BUF_SIZE];
    memset(buf, '\0', sizeof(buf));

    if (lseek(fd, pointer, SEEK_SET) == -1)
    {
        printf("Lseek failed\n");
        return 1;
    }
    while (length > 0)
    {
        int write_cnt = length < BUF_SIZE ? length : BUF_SIZE;
        if (write(fd, buf, write_cnt) == -1)
        {
            printf("Write failed\n");
            return 1;
        }
        length -= write_cnt;
    }

    stat(filename, &s);
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    int retval = 0;
    if (argc < 3)
    {
        printf("Invalid argument\n");
        return 1;
    }
    filename = argv[1];
    if (stat(filename, &s) != 0)
    {
        printf("File not found: %s\n", filename);
        return 1;
    }
    char mode = argv[2][0];
    switch (mode)
    {
    case 'i': {
        if (argc < 5)
        {
            printf("Invalid argument\n");
            retval = 1;
            break;
        }
        int pointer = stoi(argv[3]);
        char *content = argv[4];
        retval = insert(pointer, content);
        break;
    }
    case 's': {
        if (argc < 5)
        {
            printf("Invalid argument\n");
            retval = 1;
            break;
        }
        int pointer = stoi(argv[3]);
        char *content = argv[4];
        retval = substitute(pointer, content);
        break;
    }
    case 'd': {
        if (argc < 5)
        {
            printf("Invalid argument\n");
            retval = 1;
            break;
        }
        int pointer=stoi(argv[3]);
        int length=stoi(argv[4]);
        retval = delete(pointer, length);
        break;
    }
    default:
        printf("Invalid mode: %c\n", mode);
        retval = 1;
        break;
    }
    return retval;
}