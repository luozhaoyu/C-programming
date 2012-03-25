/* 
 * File:   server.c
 * Author: leonardo
 *
 * Created on 2012年3月10日, 上午1:18
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <assert.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

//#include "getaddrinfo.h"

/*
 * 
 */
int main(int argc, char** argv) {
    int fd;
    struct sockaddr_in linuxaddr;
    socklen_t socklen;
    struct addrinfo *ailist;

    char abuf[256];
    int err;

    printf("Server is started...\n");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket initial error\n");
        exit(EXIT_FAILURE);
    }

    linuxaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    unsigned int  ip;
    inet_pton(AF_INET, "127.0.0.1", &ip);
    linuxaddr.sin_addr.s_addr = ip;
    
    linuxaddr.sin_family = AF_INET;
    linuxaddr.sin_port = htons(12345);

    printf("struct sockaddr size = %lu , linuxaddr size = %lu\n", sizeof (struct sockaddr*), sizeof (linuxaddr));

    if (bind(fd, (struct sockaddr*) &linuxaddr, sizeof (linuxaddr)) < 0) {
        perror("bind error!\n");
        exit(EXIT_FAILURE);
    } else {
        struct sockaddr_in __addr;
        socklen_t __len;
        if (getsockname(fd, (struct sockaddr*) &__addr, &__len) < 0) {
            perror("bind address error!\n");
        } else {

            printf("THE BINDED ADDRESS is %s\n", inet_ntoa(__addr.sin_addr));
        }
    }

    if (listen(fd, SOMAXCONN) < 0) {
        perror("listen failed!\n");
        exit(EXIT_FAILURE);
    } else {
        int client_fd;
        struct sockaddr_in client_address;
        socklen_t address_len;
        char buf[32];
        int n;
        while (1) {
            client_fd = accept(fd, (struct sockaddr *)&client_address, &address_len);
            if (client_fd < 0) {
                perror("accept failed!\n");
            } else {
                printf("CLIENT CONNECTED SUCCESS!\n");
                while ((n = recv(client_fd, buf, 32, 0)) > 0) {
                    printf("CLIENTS SAID %i WORDS: ", n); // Is here caching something or not?
                    write(STDOUT_FILENO, buf, n);
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}

