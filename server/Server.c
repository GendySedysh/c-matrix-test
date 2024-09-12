#include "Server.h"

// ---------------------- Storage ----------------------

static void PrintStorageData(t_Storage *storage) {
	printf("Det. = %f\n", storage->storage[storage->head]);
	if (storage->iterations >= MATRIX_STORAGE) {
		double avg_det = 0;
		for (int i = 0; i < MATRIX_STORAGE; ++i) {
			avg_det += storage->storage[i];
		}
		printf("Avg. det = %f\n", avg_det / 5.0);
		printf("Del. det = %f\n\n", storage->storage[storage->tail]);
	} else {
		printf("Avg. det = N/a\n");
		printf("Del. det = N/a\n\n");
	}
}

static void getCofactor(double mat[MATRIX_SIZE][MATRIX_SIZE], 
				 double temp[MATRIX_SIZE][MATRIX_SIZE], 
                 int p, int q, int n)
{
    int i = 0, j = 0;
 
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++) 
        {
            if (row != p && col != q) 
            {
                temp[i][j++] = mat[row][col];
 
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}
 
static double determinantOfMatrix(double mat[MATRIX_SIZE][MATRIX_SIZE], int n) {
    double D = 0; 
 
    if (n == 1)
        return mat[0][0];
 
    double temp[MATRIX_SIZE][MATRIX_SIZE]; 
    int sign = 1; 
 
    for (int f = 0; f < n; f++) {
        getCofactor(mat, temp, 0, f, n);
        D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1);
 
        sign = -sign;
    }
 
    return D;
}

void AddNewDeterminant(t_Storage *storage, char *str) {
	double matrix[MATRIX_SIZE][MATRIX_SIZE];

	// Заполняем матрицу
	for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
			matrix[i][j] = str[i * MATRIX_SIZE + j] - '0';
		}
    }

	// Считаем определитель
	double determinant = determinantOfMatrix(matrix, MATRIX_SIZE);

	// Добавляем определитель в Storage и выводим данные в stdio
	storage->storage[storage->head] = determinant;
	if (storage->head == 4) {
		storage->tail = 0;
	} else {
		storage->tail = storage->head + 1;
	}

	storage->iterations++;
	PrintStorageData(storage);

	storage->head++;
    if (storage->head == MATRIX_STORAGE) {
		storage->head = 0;
	}
}

// ---------------------- Server ----------------------

void CreateSocket(t_Server *server) {
    server->listening = socket(AF_INET, SOCK_STREAM, 0);
    if (server->listening == -1)
    {
    	fprintf(stderr, "Failed to create socket.\n");
    	exit(EXIT_FAILURE);
    }
}

void BindSocket(t_Server *server) {
	server->hint.sin_family = AF_INET;
	server->hint.sin_addr.s_addr = inet_addr("127.0.0.1");
	server->hint.sin_port = htons(server->port);
	if (bind(server->listening, (struct sockaddr*)&server->hint, sizeof(struct sockaddr)) < 0)
	{
        fprintf(stderr, "Failed to bind to port.\n");
		exit(EXIT_FAILURE);
	}
}

void ListenSocket(t_Server *server) {
	if (listen(server->listening, MAX_CLIENTS) < 0)
	{
        fprintf(stderr, "Failed to listen on socket.\n");
		exit(EXIT_FAILURE);
	}
	fcntl(server->listening, F_SETFL, O_NONBLOCK);
}

void GrabConnection(t_Server *server) {
	size_t addrlen = sizeof(struct sockaddr);
	int connection = accept(server->listening, (struct sockaddr*)&server->hint, (socklen_t*)&addrlen);

	if (connection >= 0)
	{
		char	host[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(server->hint.sin_addr), host, INET_ADDRSTRLEN);
		struct pollfd	pfd;
		pfd.fd = connection;
		pfd.events = POLLIN;
		pfd.revents = 0;

		server->client_fds[server->connected_clients++] = pfd;
	}
}

void ProcessMessages(t_Server *server, t_Storage *storage) {
	int	pret = poll(server->client_fds, MAX_CLIENTS, 1);
	char	buffer[MSG_SIZE + 1];
	int		readed;

	if (pret != 0)
	{
		for (size_t i = 0; i < MAX_CLIENTS; i++)
		{
			if (server->client_fds[i].revents & POLLIN)
			{
				readed = 0;
				memset(buffer, 0, sizeof(buffer));

				if ((readed = recv(server->client_fds[i].fd, buffer, MSG_SIZE + 1, 0)) > 0)
				{
					buffer[readed] = '\0';
					AddNewDeterminant(storage, buffer);
				}
			}
			server->client_fds[i].revents = 0;
		}
	}
}

void CheckClients(t_Server *server) {
	char *buf;
	size_t retval;

	for (size_t i = 0; i < MAX_CLIENTS; i++) {
		retval = recv(server->client_fds[i].fd, &buf, 1, MSG_PEEK | MSG_DONTWAIT);

		if (retval == 0) {
			close(server->client_fds[i].fd);
            server->connected_clients--;
		}
		retval = 0;
	}
}

void ShutdownServer(t_Server *server) {
	for (size_t i = 0; i < MAX_CLIENTS; i++) {
		close(server->client_fds[i].fd);
	}
}