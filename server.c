#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define handle_error(msg) \
   do {fprintf(stderr, "%s: %s\n", (msg), strerror(errno)); exit(EXIT_FAILURE); } while (0)

int main()
{
    return EXIT_SUCCESS;
}
