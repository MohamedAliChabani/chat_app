#pragma once

#define SERVER_PORT 9090

#define BUFFSIZE 10000

#define MAX_NAME 100

extern int sockfd;

void send_client_name();
void get_name_status();
