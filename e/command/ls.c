#include "stdio.h"
#include "string.h"
#include "type.h"

#define NR_FILES 64
const int FILENAMES_LEN = (MAX_PATH + 1) * NR_FILES;

int main()
{
    char filenames[FILENAMES_LEN];
    char pathname[] = "/";
    memset(filenames, 0, sizeof(filenames));
    list(pathname, filenames);
    printf("filename    device    inode    mode    size\n");
    for (char *p = filenames; *p != 0; p += strlen(p) + 1)
    {
        struct stat st;
        if (stat(p, &st) == 0)
            printf("%s    %d    %d    %d    %d\n", p, st.st_dev, st.st_ino, st.st_mode, st.st_size);
    }
    return 0;
}