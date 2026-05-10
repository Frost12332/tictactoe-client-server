#include "transport_api.h"

#include "stdio.h"


int start_transport_level_client()
{
    struct sockaddr_in addr;
    int socket_fd;
    int result_connection;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1)
        return -1;

    result_connection = connect(socket_fd, 
                                (struct sockaddr*)&addr,
                                sizeof(struct sockaddr_in));

    if (result_connection == -1)
        return -1;

    printf("connection success");

    return socket_fd;
}

void stop_transport_level_client()
{

}