#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server_utils.h"

#define print_error_and_exit(msg) \
   do {fprintf(stderr, "%s: %s\n", (msg), strerror(errno)); exit(EXIT_FAILURE); } while (0)

int handle_port_nu(int port)
{
    if (port <= MIN_PORT - 1 || port > MAX_PORT) {
        fprintf(stderr, "Port number should be in this range (%d - %d)\n", (int) MIN_PORT, (int) MAX_PORT);
        exit(EXIT_FAILURE);
    }
    return port;
}

void print_client_addrport(int clientfd, struct sockaddr_in client_info, socklen_t *client_info_len)
{
    if (getpeername(clientfd, (struct sockaddr*) &client_info, client_info_len) < 0) {
        fprintf(stderr, "Could not get the name: %s\n", strerror(errno));
    }
    else
        printf("Client %s:%d connected to the server.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
}

client_node *create_node(int clientfd)
{
    client_node *node = malloc(sizeof(client_node));
    if (node == NULL) {
        fprintf(stderr, "Could not allocate memory space for client\n");
        return NULL;
    }
    node->fd = clientfd;
    node->next = NULL;
    return node;
}

void append_node(client_node *root, client_node *client)
{
    client_node *tmp = root;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = client;
}

void delete_list(client_node *root)
{
    while (root->next != NULL) {
        client_node *tmp_node = root->next;
        root->next = root->next->next;

        if (close(tmp_node->fd) < 0)
            fprintf(stderr, "Could not close client socket: %s\n", strerror(errno));

        free(tmp_node);
    }
    if (close(root->fd) < 0)
        fprintf(stderr, "Could not close client socket: %s\n", strerror(errno));

    free(root);
}

void handle_client(void *p_client)
{
    client_node *client = (client_node *) p_client;

    char send_buff[BUFFSIZE];
    char recv_buff[BUFFSIZE];
    
    // Get the client's name
    recv(client->fd, recv_buff, MAX_NAME, 0);
    if (strlen(recv_buff) < MIN_NAME) {
        fprintf(stderr, "\033[1;31mYou must input a name (length >= %d)\n", (int) MIN_NAME);
        exit(EXIT_FAILURE);
    }
    strncpy(client->name, recv_buff, strlen(recv_buff) - 1);
    printf("%s joined the communication\n", client->name);
    while (1) {
        // recv(client->fd, recv_buff, MAX_NAME, 0);
        // strncat(recv_buff, recv_buff, strlen(recv_buff) - 1);
    };
}
