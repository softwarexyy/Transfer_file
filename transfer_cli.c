#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "package.h"
#define oops(msg)   {perror(msg);   exit(EXIT_FAILURE);}

//开启socket连接server,返回一个socket描述符用于读写文件
int startconn(char *server_ip);
//  write to fd (a file descriptor)
void sendfile(int fd, char *file_path);

int main(int argc, char *argv[])
{
    if (argc != 3)                  //argv[1] is the server IP, argv[2] is the file path
        oops("argument error !");

    char* server_ip = argv[1];
    char* file_path = argv[2];
    int fd = startconn(server_ip);

    sendfile(fd, file_path);
    close(fd);
    printf("文件传输成功！\n");
    return 0;
}

int startconn(char *server_ip)
{
    int connsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connsock == -1)
        oops("client socket() error");

    struct sockaddr_in servaddr;     //create a server address
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    servaddr.sin_addr.s_addr = inet_addr(server_ip);

    if (connect(connsock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        oops("client connect() error");

    //  now we can write to the connect socket
    printf("Connect Success !\n");
    return connsock;
}

void sendfile(int fd, char *file_path)
{
    struct stat file_info;
    unsigned long file_len;
    if (stat(file_path, &file_info)<0)
        oops("client stat()");
    file_len = file_info.st_size;
    printf("文件长度：%lu\n", file_len);
    struct package packtosend;
    memset(&packtosend, 0, sizeof(struct package));
    packtosend.len = file_len;
    //packtosend.body = (char *)malloc(sizeof(file_len));

    int filefd;
    if ((filefd = open(file_path, O_RDONLY)) == -1)
        oops("client open()");

    if (read(filefd, (void*)packtosend.body, file_len) == -1)
        oops("client read()");

    close(filefd);
    // write the package to fd
    if ((write(fd, (void *)&packtosend, sizeof(unsigned long)+file_len)) == -1)
        oops("client write()");
}
