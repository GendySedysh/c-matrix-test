CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(MatrixClientServer C)

ADD_EXECUTABLE(Server server/main.c server/Server.c server/Server.h)
target_compile_options(Server PRIVATE -Wall -Wextra -Werror)


ADD_EXECUTABLE(Client client/main.c client/Client.c client/Client.h)
target_compile_options(Client PRIVATE -Wall -Wextra -Werror)