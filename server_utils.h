#pragma once

#define SERVER_PORT 9090

#define MIN_NAME 2
#define MAX_NAME 100

#define ERROR_BUFFSIZE 100
#define BUFFSIZE 10000

typedef struct client_node {
    int fd;
    char name[MAX_NAME];
    struct client_node *next;
} client_node;

int handle_port_nu(int port);
void print_client_addrport(int clientfd, struct sockaddr_in client_info, socklen_t *client_info_len);
client_node *create_node(int clientfd);
void append_node(client_node *root, client_node *new_node);
void delete_list(client_node *root);
static void get_client_name(client_node *client);
void handle_client(void *p_client);
