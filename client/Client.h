#ifndef CLIENT_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define ONLINE 1
#define OFFLINE 0
#define MSG_SIZE 36

typedef struct s_Client
{
    int sock;
    int port;
    struct sockaddr_in hint;
} t_Client;

void CreateSocket(t_Client *client);
void SetUpSocket(t_Client *client);
void ConnectToServer(t_Client *client);
void SendMessege(t_Client *client);
void DisconnectFromServer(t_Client *client);
void WaitTimeout(int timeout);

#endif