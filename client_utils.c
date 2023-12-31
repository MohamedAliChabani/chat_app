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

#define print_error(msg) \
   do {fprintf(stderr, "\033[1;31m%s: %s\n", (msg), strerror(errno));} while (0)

void send_client_name()
{
    char name[MAX_NAME];
    printf("Enter your name: ");
    fgets(name, MAX_NAME, stdin);
    if (send(sockfd, name, MAX_NAME, 0) < 0) {
        print_error("Could not send client usename to server");
    }
}

void get_name_status()
{
    char recv_buff[BUFFSIZE];
    recv(sockfd, recv_buff, BUFFSIZE, 0);
    printf("%s", recv_buff);
}

void send_handler() {
    while (1) {
        fflush(stdout);
        char send_msg[BUFFSIZE] = {};
        while (fgets(send_msg, BUFFSIZE, stdin) != NULL) {
            if (strlen(send_msg) == 0)
                fflush(stdout);
            else
                break;
        }
        send(sockfd, send_msg, BUFFSIZE, 0);
    }
}

void recv_handler()
{
    while (1) {
        fflush(stdout);
        char recv_buff[BUFFSIZE] = {};
        recv(sockfd, recv_buff, BUFFSIZE, 0);
        printf("%s\n", recv_buff);
        fflush(stdout);
    }
}
