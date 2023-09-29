#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define print_error_and_exit(msg) \
   do {fprintf(stderr, "%s: %s\n", (msg), strerror(errno)); exit(EXIT_FAILURE); } while (0)

extern int sockfd;

void exit_at_ctrl_c(int sig)
{
    printf("\nExecution Interrupted, all communications have ended\n");

    if (close(sockfd) < 0)
        print_error_and_exit("...Could not close server socket\n");

    exit(EXIT_SUCCESS);
}

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
