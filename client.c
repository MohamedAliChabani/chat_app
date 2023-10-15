#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#include "client_utils.h"

#define print_error_and_exit(msg) \
   do {fprintf(stderr, "\033[1;31m%s: %s\n", (msg), strerror(errno)); exit(EXIT_FAILURE); } while (0)

int sockfd;

static void exit_at_ctrl_c(int sig)
{
    if (close(sockfd) < 0)
        print_error_and_exit("...Could not close client socket");

    printf("You exited the chatroom\n");
    exit(EXIT_SUCCESS);
}

int main()
{
    // Handle Ctrl-c interruption
    signal(SIGINT, exit_at_ctrl_c);

    struct sockaddr_in server_info = {0};
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(SERVER_PORT);
    socklen_t server_info_len = sizeof(server_info);

    struct sockaddr_in client_info = {0};
    socklen_t client_info_len = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(sockfd, (struct sockaddr *)&server_info, server_info_len) < 0)
        print_error_and_exit("Could not connect to the server");

    send_client_name();
    get_name_status();
}
