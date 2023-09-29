#ifndef SERVER_UTILS_H_
#define SERVER_UTILS_H_

void exit_at_ctrl_c(int sig);
int handle_port_nu(int port);
void print_client_addrport(int clientfd, struct sockaddr_in client_info, socklen_t *client_info_len);

#endif //SERVER_UTILS_H_
