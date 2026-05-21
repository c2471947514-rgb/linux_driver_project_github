#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> // 必须包含：用于 memcpy

int main(int argc, char *argv[])
{   
    int ret = 0;
    int fd = 0;
    char *filename;
    char readbuf[100], writebuf[100];
    static char usrdata[] = {"usr data!"};

    // 检查参数：程序名 + 设备名 + 读写指令(1或2)
    if(argc != 3){
        printf("Error usage!\r\n");
        printf("Usage: %s <dev_path> <1 for read, 2 for write>\r\n", argv[0]);
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("cannot open file %s\r\n", filename);
        return -1;
    }

    /* 1. 读操作 */
    if(atoi(argv[2]) == 1){
        ret = read(fd, readbuf, 50);
        if(ret < 0){
            printf("read file %s failed!\r\n", filename);
        } else {
            // 修正：打印读到的数据 readbuf
            printf("APP read data: %s\r\n", readbuf);
        }
    }

    /* 2. 写操作 */
    if(atoi(argv[2]) == 2){
        // 修正：在写入前准备好数据
        memcpy(writebuf, usrdata, sizeof(usrdata));
        ret = write(fd, writebuf, 50);
        if(ret < 0){
            printf("write file %s failed!\r\n", filename);
        } else {
            printf("APP write data ok!\r\n");
        }
    }

    ret = close(fd);
    if(ret < 0){
        printf("close file %s failed!\r\n", filename);
    }

    return 0;
}