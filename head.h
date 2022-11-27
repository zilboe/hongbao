#ifndef __HEAD_H__
#define __HEAD_H__
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
typedef struct CLIENT
{
    int fd;
    struct sockaddr_in client;
    struct CLIENT *next;
}client_t;
typedef struct PACKET
{
    int clientSize;
    pthread_mutex_t lock;
    client_t client;
}pack_t;
typedef struct RedPack
{
    char name[20];
    char message[128];
    int number;
    int size;
    int type;// 1是发红包,2是抢红包,3是初始化接收的一个信息
}redpack_t;
#endif