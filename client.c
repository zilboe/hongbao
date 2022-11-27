#include "head.h"
int sock;
void *pthread_recv(void *arg)
{
    redpack_t pack;
    bzero(&pack, 0);
    while(1)
    {
        recv(sock, &pack, sizeof(pack), 0);
        switch(pack.type)
        {
            case 1:
            {

                break;
            }
            case 2:
            {

                break;
            }
            case 3:
            {
                system("clear");
                printf("当前房间【%d】玩家\n",pack.size);
                printf("%s\n", pack.message);
                break;
            }
        }
    }
}

void *pthread_send(void *arg)
{
    while(1)
    {
        
    }
}

int main()
{
    redpack_t pack;
    bzero(&pack, 0);
    pack.name[strlen(pack.name)-1] = '\0';
    int quit = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverSocket;
    bzero(&serverSocket, 0);
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverSocket.sin_port = htons(8888);
    socklen_t serverLen = sizeof(struct sockaddr_in);
    connect(sock, (struct sockaddr *)&serverSocket, serverLen);
    printf("请输入你的名字:");
    fgets(pack.name, sizeof(pack.name), stdin);
    pack.name[strlen(pack.name)] = '\0';
    send(sock, &pack, sizeof(pack), 0);
    pthread_t recvPthread, sendPthread;
    pthread_create(&recvPthread, NULL, pthread_recv, NULL);
    pthread_create(&sendPthread, NULL, pthread_send, NULL);
    while(!quit);
    return 0;
}