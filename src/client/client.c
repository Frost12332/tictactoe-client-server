#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../shared/buffer.h"
#include "../shared/base.h"
#include "../shared/list.h"


int main()
{
    int result_connection;
    struct sockaddr_in addr;
    int socket_fd;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    result_connection = connect(socket_fd, 
                                (struct sockaddr*)&addr,
                                sizeof(struct sockaddr_in));

    

    return EXIT_SUCCESS;
}