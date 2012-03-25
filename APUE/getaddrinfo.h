/* 
 * File:   getaddrinfo.h
 * Author: leonardo
 *
 * Created on 2012年3月10日, 下午1:14
 */

#ifndef GETADDRINFO_H
#define	GETADDRINFO_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* GETADDRINFO_H */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
int netinfo(const char *host, const char* service, struct addrinfo **res);