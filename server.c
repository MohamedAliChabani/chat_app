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

#include "server_utils.h"

int sockfd;
client_node *root_node = NULL;

#define print_error_and_exit(msg) \
   do {fprintf(stderr, "\033[1;31m%s: %s\n", (msg), strerror(errno)); exit(EXIT_FAILURE); } while (0)

static void exit_at_ctrl_c(int sig)
{
    printf("\nExecution Interrupted, all communications have ended\n");

    if (root_node != NULL)
        delete_list(root_node);

    if (close(sockfd) < 0)
        print_error_and_exit("...Could not close server socket");

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
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

    // Binding the socket
    if (bind(sockfd, (struct sockaddr *)&server_info, server_info_len) < 0)
        print_error_and_exit("Could not assign a name to the socket");

    // Listening to connections
    if (listen(sockfd, 5) < 0)
        print_error_and_exit("Could not assign a name to the socket");

    // Printing server informations
    printf("Started server session %s:%d\n", inet_ntoa(server_info.sin_addr), SERVER_PORT);

    while (1) {
        int clientfd = accept(sockfd, (struct sockaddr *)&client_info, &client_info_len);
        print_client_addrport(clientfd, client_info, &client_info_len);

        client_node *new_client = create_node(clientfd);
        if (root_node == NULL)
            root_node = new_client;
        else
            append_node(root_node, new_client);

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, (void *)handle_client, (void *)new_client) != 0)
            print_error_and_exit("Could not create client threads");
    }

    return EXIT_SUCCESS;
}
