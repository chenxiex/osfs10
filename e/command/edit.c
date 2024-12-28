#include "stdio.h"
#include "type.h"
#include "string.h"

const int BUF_SIZE = 1024;

struct stat s;
char *filename;

PRIVATE unsigned int stou(char *str)
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

    char buf[BUF_SIZE];
    int pointer = s.st_size - BUF_SIZE > 0 ? s.st_size - BUF_SIZE : 0;
    int fsize = s.st_size;
    while (1)
    {
        lseek(fd, pointer, SEEK_SET);
        int read_cnt = BUF_SIZE > s.st_size - pointer ? s.st_size - pointer : BUF_SIZE;
        read_cnt = read(fd, buf, read_cnt);
        for (int i = read_cnt - 1; i >= 0; i--)
        {
            if (buf[i] == 0)
            {
                size--;
                fsize--;
            }
            else
            {
                break;
            }
        }
        if (pointer == 0)
        {
            break;
        }
        else
        {
            pointer = pointer - BUF_SIZE > 0 ? pointer - BUF_SIZE : 0;
        }
    }
    if (size <= 0)
    {
        close(fd);
        return fsize;
    }

    if (lseek(fd, s.st_size, SEEK_SET) == -1)
    {
        printf("Failed to enlarge file\n");
        return -1;
    }

    memset(buf, 0, sizeof(buf));

    while (size > 0)
    {
        int write_cnt = size < BUF_SIZE ? size : BUF_SIZE;
        if (write(fd, buf, write_cnt) == -1)
        {
            printf("Failed to enlarge file\n");
            return -1;
        }
        size -= write_cnt;
    }

    stat(filename, &s);
    close(fd);
    return s.st_size;
}

PRIVATE int substitute(unsigned int pointer, char *content)
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

PRIVATE int insert(unsigned int pointer, char *content)
{
    int fd = open(filename, O_RDWR);
    int content_len = strlen(content);
    int org_size = s.st_size;
    if (fd == -1)
    {
        printf("Failed to open file\n");
        return 1;
    }

    if ((org_size = enlarge_file(pointer < org_size ? content_len : pointer + content_len - org_size)) == -1)
    {
        return 1;
    }

    if (pointer < org_size)
    {
        char buf[BUF_SIZE];
        int move_cnt = org_size - pointer < BUF_SIZE ? org_size - pointer : BUF_SIZE;
        int lpointer = org_size - move_cnt;
        assert(lpointer >= 0);
        int rpointer = lpointer + content_len;
        while (move_cnt > 0)
        {
            lseek(fd, lpointer, SEEK_SET);
            move_cnt = read(fd, buf, move_cnt);
            lseek(fd, rpointer, SEEK_SET);
            write(fd, buf, move_cnt);
            move_cnt = lpointer - pointer < BUF_SIZE ? lpointer - pointer : BUF_SIZE;
            lpointer -= move_cnt;
            rpointer = lpointer + content_len;
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

PRIVATE int delete(unsigned int pointer, unsigned int length)
{
    int fd = open(filename, O_RDWR);
    int fd2 = open(filename, O_RDWR);
    if (fd == -1 || pointer >= s.st_size || fd2 == -1)
    {
        return 1;
    }
    if (pointer + length > s.st_size)
    {
        length = s.st_size - pointer;
    }

    char buf[BUF_SIZE];
    int rpointer = pointer + length;
    int move_cnt = s.st_size - rpointer < BUF_SIZE ? s.st_size - rpointer : BUF_SIZE;
    lseek(fd, rpointer, SEEK_SET);
    lseek(fd2, pointer, SEEK_SET);
    while (move_cnt > 0)
    {
        move_cnt = read(fd, buf, move_cnt);
        write(fd2, buf, move_cnt);
        rpointer += move_cnt;
        move_cnt = s.st_size - rpointer < BUF_SIZE ? s.st_size - rpointer : BUF_SIZE;
    }

    memset(buf, 0, sizeof(buf));
    int zero_cnt = length;
    while (zero_cnt > 0)
    {
        int write_cnt = zero_cnt < BUF_SIZE ? zero_cnt : BUF_SIZE;
        if (write(fd2, buf, write_cnt) == -1)
        {
            return 1;
        }
        zero_cnt -= write_cnt;
    }

    stat(filename, &s);
    close(fd2);
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
        unsigned int pointer = stou(argv[3]);
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
        unsigned int pointer = stou(argv[3]);
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
        unsigned pointer = stou(argv[3]);
        unsigned length = stou(argv[4]);
        retval = delete (pointer, length);
        break;
    }
    default:
        printf("Invalid mode: %c\n", mode);
        retval = 1;
        break;
    }
    return retval;
}