#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <math.h>

#define ONLINE 1
#define OFFLINE 0
#define MSG_SIZE 36
#define MAX_CLIENTS 10
#define MATRIX_STORAGE 5
#define MATRIX_SIZE 6

typedef struct s_Storage {
    double storage[MATRIX_STORAGE];
    int head;
    int tail;
    int iterations;
} t_Storage;

void AddNewDeterminant(t_Storage *storage, char *str);

typedef struct s_Server {
    int port;
    int listening;
    struct sockaddr_in hint;
    struct pollfd client_fds[MAX_CLIENTS];
    int connected_clients;
} t_Server;

void CreateSocket(t_Server *server);
void BindSocket(t_Server *server);
void ListenSocket(t_Server *server);
void GrabConnection(t_Server *server);
void ProcessMessages(t_Server *server, t_Storage *storage);
void CheckClients(t_Server *server);
void ShutdownServer(t_Server *server);