#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

#define MAX_NAME 100

typedef struct client_node {
    int fd;
    char name[MAX_NAME];
    struct client_node *next;
} client_node;

int handle_port_nu(int port);
void print_client_addrport(int clientfd, struct sockaddr_in client_info, socklen_t *client_info_len);
void add_client(int clientfd);
void delete_list(client_node *head);

#endif //SERVER_UTILS_H_
