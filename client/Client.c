#include "Client.h"

void CreateSocket(t_Client *client) {
    client->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sock == -1)
    {
        fprintf(stderr, "Failed to create socket.\n");
    	exit(EXIT_FAILURE);
    }
}

void SetUpSocket(t_Client *client){
	client->hint.sin_family = AF_INET;
	client->hint.sin_addr.s_addr = inet_addr("127.0.0.1");
	client->hint.sin_port = htons(client->port);
}

void ConnectToServer(t_Client *client) {
    int connectRes = connect(client->sock, (struct sockaddr*)&client->hint, sizeof(struct sockaddr));
    if (connectRes == -1) {
        fprintf(stderr, "Failed to connect.\n");
    	exit(EXIT_FAILURE);
    }
    printf("Successfully connected.\n");
}

void SendMessege(t_Client *client) {
    char messege[MSG_SIZE + 1];
    srand(time(NULL));

    for (int i = 0; i < MSG_SIZE; ++i) {
        messege[i] = '0' + rand() % 10;
    }
    messege[MSG_SIZE] = '\0';
    send(client->sock, messege, MSG_SIZE + 1, 0);
}

void DisconnectFromServer(t_Client *client) {
    close(client->sock);
    printf("Disconnected.\n");
}

void WaitTimeout(int timeout) {
    sleep(timeout);
}
