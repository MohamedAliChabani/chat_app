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

static void get_client_name(client_node *client)
{
    // trimming the last character of the client's name it it's a new_line char
    char *name = client->name;
    name[strlen(name)-1] = (name[strlen(name)-1] == '\n') ? '\0' : name[strlen(name) - 1];
    if (strlen(name) <= MIN_NAME || strlen(name) > MAX_NAME) {
        // fprintf(stderr, "\033[1;31mYou must input a valid name (%d < length < %d)\n", (int) MIN_NAME, (int) MAX_NAME);
        char err_msg[ERROR_BUFFSIZE];
        snprintf(err_msg, sizeof(err_msg), "You must input a valid name (%d < length < %d)\n", (int) MIN_NAME, (int) MAX_NAME);
        send(client->fd, err_msg, strlen(err_msg), 0);
        name = NULL;
    }
    else {
        char send_buff[BUFFSIZE];
        snprintf(send_buff, sizeof(send_buff), "You were assigned the name: %s\n", client->name);
        send(client->fd, send_buff, strlen(send_buff), 0);
    }
}

void handle_client(void *p_client)
{
    client_node *client = (client_node *) p_client;

    char send_buff[BUFFSIZE];
    char recv_buff[BUFFSIZE];
    
    // Get the client's name
    recv(client->fd, client->name, MAX_NAME, 0);
    get_client_name(client);
    while (1) {
        // recv(client->fd, recv_buff, MAX_NAME, 0);
        // strncat(recv_buff, recv_buff, strlen(recv_buff) - 1);
    };
}
