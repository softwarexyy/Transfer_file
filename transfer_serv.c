#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "package.h"

#define oops(msg)   {perror(msg);   exit(EXIT_FAILURE);}

int recvsock();
void recvfile(int fd, char *file_path);

int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("argument error!\n");
        exit(EXIT_FAILURE);
    }
    char *newfile_path = argv[1];
    int fd = recvsock();
    recvfile(fd, newfile_path);
    printf("文件传输成功！\n");
    return 0;
}

void recvfile(int fd, char* file_path)
{
    unsigned long file_len_tmp;
    if ((read(fd, &file_len_tmp, sizeof(unsigned long))) == -1)
        oops("server read1()");
    int file_len = (int)file_len_tmp;
    printf("文件长度：%d\n", file_len);

    char *recvbuf = (char*)malloc(file_len);
    if ((read(fd, (void *)recvbuf, file_len)) == -1)
        oops("server read2()");

    // write to my new file:
    int newfile = open(file_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if (newfile == -1)
        oops("server open()");
    ssize_t nwrite = write(newfile, recvbuf, file_len);
    if (nwrite < file_len)
        oops("server write()");
}

int recvsock()
{
    int listen_sock;           //socket for listenning
    int conn_sock;           //socket for transfer data

    if ((listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        oops("server socket()");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind an address to the socket:
    if (bind(listen_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        oops("server bind()");

    // listen:
    if(listen(listen_sock, SOMAXCONN) < 0)
        oops("server listen()");

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    if ((conn_sock = accept(listen_sock, (struct sockaddr*)&peeraddr, &peerlen)) < 0 )
        oops("server accept()");

    // now we can get data from conn_sock
    printf("Connect Success !\n");
    return conn_sock;
}
