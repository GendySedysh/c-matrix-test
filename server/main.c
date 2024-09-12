#include "Server.h"

static int status = ONLINE;

void handle_sigint(int signum) {
    (void) signum;
    status = OFFLINE;
}

int main(int argc, char **argv) {
    signal(SIGINT, handle_sigint);

    if (argc != 2) {
        fprintf(stderr, "Usage: ./server <port>\n");
		exit(EXIT_FAILURE);
	}

	int port = atoi(argv[1]);

    if (port < 1024 || port > 49000) {
		fprintf(stderr, "Wrong port! Required from 1025 to 49000\n");
		exit(EXIT_FAILURE);
	}

    t_Storage storage;
    storage.head = 0;
    storage.tail = 0;
    storage.iterations = 0;

    t_Server server;
    server.port = port;
    server.listening = 0;
    server.connected_clients = 0;

    // Настраиваем сокет
    CreateSocket(&server);
    BindSocket(&server);
    ListenSocket(&server);

    // Принимаем сообщения
    while (status) {
        GrabConnection(&server);
        ProcessMessages(&server, &storage);
        CheckClients(&server);
    }

    ShutdownServer(&server);
}