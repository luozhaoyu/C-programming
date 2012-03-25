/* 
 * File:   16-3-server.c
 * Author: leonardo
 *
 * Created on 2012年3月13日, 下午10:52
 */

/*
 * TODO:
 * 使用printf究竟会不会冲洗缓冲区？
 * write read buf
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <fcntl.h>

#define CONCURREN_LEVEL 5
#define BUFSIZE 128

static int log;

/*
 * 
 */
struct sockaddr_in *generateSockaddr(const char* __ip, const unsigned short __port) {
    struct sockaddr_in *newsock;
    newsock = (struct sockaddr_in *) malloc(sizeof (struct sockaddr_in));
    bzero(newsock, sizeof (struct sockaddr_in));
    newsock->sin_family = AF_INET;
    newsock->sin_port = htons(__port);
    if (inet_pton(AF_INET, __ip, &(newsock->sin_addr.s_addr)) != 1) {
        perror("something wrong with ip address convertion!\n");
        return NULL;
    } else {
/*
        printf("DEBUG INFO: SIZEOF sockaddr_in IS %lu\n", sizeof (newsock));
*/
        printf("listening unsigned short __port = %u\n", __port);
        return newsock;
    }
}

int printSockaddr(struct sockaddr_in *__socket_address) {
    struct sockaddr_in tmp;
    char buf[256];
    bzero((void*) buf, (size_t) 256);
    inet_ntop(AF_INET, &(__socket_address->sin_addr.s_addr), buf, 256);
    printf("IP = %s, port = %i, size = %lu\n", buf, __socket_address->sin_port, sizeof (__socket_address));
}

int setSocketReuse(int *__socket) {
    int reuse = 1;
    int ret;
    ret = setsockopt(*__socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (int));
    if (ret < 0) {
        perror("setsocket option error!\n");
        return -1;
    } else
        return 0;
}

int readSocket(int __socket) {
    int s;
    char buf[BUFSIZE];
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork error:");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /*
         child process
         */
        struct sockaddr_in client_socket_address;
        socklen_t client_len;
        s = accept(__socket, (struct sockaddr *) &client_socket_address, &client_len);
        bzero(buf, BUFSIZE);
        while (read(s, buf, BUFSIZE) > 0) {
            write(log, buf, BUFSIZE);
            printf("CLIENT SAID: %s", buf);
            bzero(buf, BUFSIZE);
            strcpy(buf, "oswidjfnjdf");
            write(s, buf, BUFSIZE);
            bzero(buf, BUFSIZE);
        }
        printf("CONNECTION CLOSED.\n");
        close(s);
        exit(EXIT_SUCCESS);
    } else {
        /*
         parent process
         * do nothing?
         */
        printPid();
        printf("is exiting!\n");
/*
        waitpid(pid, NULL, 0);
*/
    }
}

int writeSocket(int __socket) {
    int s;
    char buf[BUFSIZE];
    if (fork() < 0) {
        perror("fork error:");
        exit(EXIT_FAILURE);
    } else if (fork() == 0) {
        /*
         child process
         */
        struct sockaddr_in client_socket_address;
        socklen_t client_len;
        s = accept(__socket, (struct sockaddr *) &client_socket_address, &client_len);
        bzero(buf, BUFSIZE);
        strcpy(buf, "what a fucking day!");
        if (write(s, buf, 64) < 0) {
            perror("write to client error:");
            close(s);
            exit(EXIT_FAILURE);
        } else {
            printf("Write to client successfully!\n");
            close(s);
            exit(EXIT_SUCCESS);
        }
    } else {
        /*
         parent process
         * do nothing?
         */
    }
    close(s);
}

int resetSocketset(int *__server_fd, fd_set *__readset, fd_set *__writeset, fd_set *__exceptset) {
    FD_ZERO(__readset);
    FD_ZERO(__writeset);
    FD_ZERO(__exceptset);
    
    int i;
    for (i = 0; i < 4; i++) {
        FD_SET(__server_fd[i], __readset);
        FD_SET(__server_fd[i], __writeset);
        FD_SET(__server_fd[i], __exceptset);
    }
}

int printPid() {
    printf("Current pid = %u", getpid());
    return 0;
}

int main(int argc, char** argv) {
    int server_fd[CONCURREN_LEVEL];
    int max_fd = 0;
    fd_set readset, writeset, exceptionset;
    struct sockaddr_in *socket_address;
    int ret;
    int i;
    char *ip[CONCURREN_LEVEL];
    unsigned short ports[CONCURREN_LEVEL] = {100, 800, 1500, 13, 400};
    ip[0] = "127.0.0.1";
    ip[1] = "127.0.0.2";
    ip[2] = "127.0.0.1";
    ip[3] = "192.168.0.31";
    log = open("/var/log/test.log", O_RDWR | O_CREAT);

    for (i = 0; i < 4; i++) {
        server_fd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd[i] < 0) {
            perror("socket initial error!\n");
            exit(EXIT_FAILURE);
        } else {
            if (server_fd[i] > max_fd)
                max_fd = server_fd[i];
        }
        socket_address = generateSockaddr(ip[i], ports[i]);
        ret = bind(server_fd[i], (struct sockaddr *) socket_address, sizeof (*socket_address));
        if (ret < 0) {
            perror("bind error!");
            printf("error info is %s\nfd = %i\n", strerror(ret), server_fd[i]);
            printSockaddr(socket_address);
            exit(EXIT_FAILURE);
        }
        listen(server_fd[i], 5);
    }

    struct sockaddr_in client_address;
    socklen_t client_address_len;
    int ready_fd[CONCURREN_LEVEL];

    printf("The max_fd = %i\n", max_fd);
    while (1) {
        resetSocketset(server_fd, &readset, &writeset, &exceptionset);
        if (pselect(max_fd + 1, &readset, &writeset, NULL, NULL, NULL) < 0) {
            perror("pselect error:");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < 4; i++) {
            if (FD_ISSET(server_fd[i], &writeset)) {
                printf("server_fd %i is writable...", i);
                writeSocket(server_fd[i]);
            } else if (FD_ISSET(server_fd[i], &readset)) {
                printf("server_fd %i is readable...", i);
                printPid();
                readSocket(server_fd[i]);
            }
        }
        printf("Another while pid = %u\n", getpid());
    }
    for (i = 0; i < 4; i++)
        close(server_fd[i]);
    return (EXIT_SUCCESS);
}