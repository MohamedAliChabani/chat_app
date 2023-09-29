#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"

#define print_error_and_exit(msg) \
   do {fprintf(stderr, "%s: %s\n", (msg), strerror(errno)); exit(EXIT_FAILURE); } while (0)

int sockfd;

void exit_at_ctrl_c(int sig)
{
    printf("\nExecution Interrupted, the server is down\n");

    if (close(sockfd) < 0)
        print_error_and_exit("...Could not close server socket");

    exit(EXIT_SUCCESS);
}

int main()
{
    // Handle Ctrl-c interruption
    signal(SIGINT, exit_at_ctrl_c);

    // Server and client socket information
    struct sockaddr_in server_info = {0};
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(SERVER_PORT);
    socklen_t server_info_len = sizeof(server_info);

    struct sockaddr_in client_info = {0};
    socklen_t client_info_len = {0};

    // Creating server socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        print_error_and_exit("Could not create server socket");

    if (bind(sockfd, (struct sockaddr *)&server_info, server_info_len) < 0)
        print_error_and_exit("Could not assign a name to the socket");

    if (listen(sockfd, 5) < 0)
        print_error_and_exit("Could not assign a name to the socket");

    return EXIT_SUCCESS;
}
