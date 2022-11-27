
#include "head.h"
int sock;
pack_t client;
int redNumber;
void *pthread_recv_send(void *arg)
{
    int fd = *(int *)arg;
    redpack_t *recvPack = (redpack_t *)malloc(sizeof(redpack_t));
    memset(recvPack, 0, sizeof(redpack_t));
    recv(fd, recvPack->name, sizeof(recvPack->name), 0);
    recvPack->size = client.clientSize;
    recvPack->type = 3;
    if (strlen(recvPack->name)!=0)
    {
        sprintf(recvPack->message, "%s进入抢红包群", recvPack->name);
        printf("%s进入抢红包群\n", recvPack->name);
        client_t *p = &client.client;
        pthread_mutex_lock(&client.lock);
        while(p->next!=NULL)
        {
            if (p->fd != fd)
            {
                send(p->fd, recvPack, sizeof(redpack_t), 0);
                p = p->next;
            }
            else
            {
                p = p->next;
            }
        }
        pthread_mutex_unlock(&client.lock);
    }

}

void *pthread_connect(void *arg)
{
    // 客户端结构体
    struct sockaddr_in clientSocket;
    bzero(&clientSocket, 0);
    socklen_t clientLen = sizeof(struct sockaddr_in);
    while (1)
    {
        int acceptfd = accept(sock, (struct sockaddr *)&clientSocket, &clientLen);
        pthread_mutex_lock(&client.lock);
        // 如果客户端链表为空
        if (client.clientSize <= 0)
        {
            client.clientSize++;
            client.client.client = clientSocket;
            client.client.next = NULL;
            client.client.fd = acceptfd;
        }
        else
        {
            client_t *addClient = (client_t *)malloc(sizeof(client_t));
            addClient->client = clientSocket;
            addClient->fd = acceptfd;
            addClient->next = NULL;
            client_t *p = &client.client;
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = addClient;
            client.clientSize++;
        }
        pthread_mutex_unlock(&client.lock);
        pthread_t recvSendPack;
        pthread_create(&recvSendPack, NULL, pthread_recv_send, (void *)&acceptfd);
    }
}

int main(int argc, const char *argv[])
{
    redNumber = 0;
    bzero(&client, 0);
    int quit = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverSocket;
    bzero(&serverSocket, 0);
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverSocket.sin_port = htons(8888);
    socklen_t serverSockLen = sizeof(struct sockaddr_in);
    bind(sock, (struct sockaddr *)&serverSocket, serverSockLen);
    listen(sock, 10);
    pthread_t connectPthread;
    pthread_create(&connectPthread, NULL, pthread_connect, NULL);
    pthread_detach(connectPthread);
    while (!quit)
        ;
    return 0;
}