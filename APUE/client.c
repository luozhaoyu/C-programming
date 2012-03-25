#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "getaddrinfo.h"

main(int argc, char** argv) {
    int fd;
    struct sockaddr_in server_address;
    struct addrinfo *aip, *ailist;
    char buf[32];

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket initial failed!\n");
    }

    if (netinfo(argv[1], argv[2], &ailist) != 0) {
        perror("IP address or port missing! OR ANYTHING ELSE!\n");
        exit(EXIT_FAILURE);
    }

    aip = ailist;

    if (connect(fd, aip->ai_addr, aip->ai_addrlen) < 0) {
        perror("client connect failed!\n");
        exit(EXIT_FAILURE);
    } else {
/*
 only connect the first address
*/
        struct in_addr address;
        struct sockaddr_in *__addr;
        __addr = (struct sockaddr_in*) aip->ai_addr;
        address = __addr->sin_addr;
        printf("GET ADDR %s\n", inet_ntop(AF_INET, &address, buf, INET_ADDRSTRLEN));
    }

    
    bzero(buf, 32);
    int n;
    while (fgets(buf, 32, stdin)) {
        printf("going to send: %s length = %lu\n", buf, strlen(buf));
        if ((n = send(fd, buf, strlen(buf), 0)) < 0) {
            perror("send failed!\n");
            printf("DATA fail to send is %s\n", buf);
            close(fd);
            exit(EXIT_FAILURE);
        } else {
            printf("start recv!\n");
            if ((n = recv(fd, buf, 32, 0)) < 0) {
                perror("recv failed!\n");
                close(fd);
                exit(EXIT_FAILURE);
            } else {
              printf("RECEIVED DATA LEN = %i : %s\n", n, buf);  
            }
        }
    }
}