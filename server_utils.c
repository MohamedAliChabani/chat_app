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

extern client_node *root_node;

int handle_port_nu(int port)
{
    if (port <= 1024 || port > 65535) {
        fprintf(stderr, "Port number should be in this interval (1025 - 65535)\n");
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

void append_node(client_node *head, client_node *client)
{
    client_node *tmp = head;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = client;
}

void add_client(int clientfd)
{
    if (root_node == NULL) {
        root_node = create_node(clientfd);
    }
    else {
        client_node *new_node = create_node(clientfd);
        append_node(root_node, new_node);
    }
}

void delete_list(client_node *head)
{
    while (head->next != NULL) {
        client_node *tmp_node = head->next;
        head->next = head->next->next;
        if (close(tmp_node->fd) < 0) {
            fprintf(stderr, "Could not close client socket: %s\n", strerror(errno));
        } 
        free(tmp_node);
    }
    if (close(head->fd) < 0) {
        fprintf(stderr, "Could not close client socket: %s\n", strerror(errno));
    }
    free(head);
}

void print_list(client_node *head)
{
    while (head != NULL) {
        printf("%d - ", head->fd);
        head = head->next;
    }
    printf("\n");
}
