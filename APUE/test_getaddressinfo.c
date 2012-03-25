/* 
 * File:   testgetaddress.c
 * Author: leonardo
 *
 * Created on 2012年3月10日, 上午11:54
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>

#include "getaddrinfo.h"

/*
 * 
 */
int main(int argc, char** argv) {
    struct addrinfo *ailist;
    char abuf[256];
    int err;
    
    if (err = netinfo("g.co", "www", &ailist) != 0)
    {
        printf("getaddrinfo function failed\n");
        printf("DETAILS: %s\n", gai_strerror(err));
    } else
    {
        struct addrinfo *aip;
        struct sockaddr_in *__addr;
        struct in_addr address;
        aip = ailist;
        while (aip)
        {
            __addr = (struct sockaddr_in*)aip->ai_addr;
            address = __addr->sin_addr;
            printf("GET ADDR %s\n", inet_ntop(AF_INET, &address, abuf, INET_ADDRSTRLEN));
            aip = aip->ai_next;
        }
    }
    return (EXIT_SUCCESS);
}

