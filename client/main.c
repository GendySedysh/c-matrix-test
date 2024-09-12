#include "Client.h"

static int status = ONLINE;

void handle_sigint(int signum) {
    (void) signum;
    status = OFFLINE;
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);

    if (argc != 2 && argc != 3)
	{
        fprintf(stderr, "Usage: ./client <port> (time)\n (time) - optional in seconds\n");
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

    if (port < 1024 || port > 49000)
	{
		fprintf(stderr, "Wrong port! Required from 1025 to 49000\n");
		exit(EXIT_FAILURE);
	}

    t_Client client;
    client.sock = 0;
    client.port = port;

    int timeout = 0;
    if (argc == 3) {
        timeout = atoi(argv[2]);
        if (timeout <= 0) {
            fprintf(stderr, "Wrong time argument.(should be int > 0)\n");
            exit(EXIT_FAILURE);
        }
    }

    // Создаём подключение
    CreateSocket(&client);
    SetUpSocket(&client);
    ConnectToServer(&client);

    // посылаем матрицу по таймуату/нажатию клавиши
    while (status)
    {
        if (timeout == 0) {
            getchar();
        } else {
            WaitTimeout(timeout);
        }
        SendMessege(&client);
    }
    
    DisconnectFromServer(&client);

    return 0;
}