#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/select.h>


#include "../shared/buffer.h"
#include "../shared/base.h"
#include "../shared/list.h"


enum
{
    queue_connection = 5
};





int main()
{
    struct sockaddr_in addr;
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct timespec timeout;
    fd_set read_fds, write_fds;

    if (socket_fd == -1)
        exit(-1);
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

    int result;

    result = bind(socket_fd, 
                       (struct sockaddr*)&addr,
                       sizeof(struct sockaddr_in));


    if (result == -1)
        exit(-1);

    result = listen(socket_fd, queue_connection);


    if (result == -1)
        exit(-1);
    int max_d = socket_fd;
    for(;;)
    {
        timeout.tv_sec = 5;
        timeout.tv_nsec = 0;

        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);

        FD_SET(socket_fd, &read_fds);


        result = select(max_d + 1, &read_fds, &write_fds,
                         NULL, &timeout);

        if(result == 0)
        {
            printf("Time out\n");
        }
        else if (FD_ISSET(socket_fd, &read_fds))
        {
            int accept_result = accept(socket_fd, NULL, NULL);
            
            if (accept_result != -1)
            {
                printf("receive new connection\n");
                
                if (accept_result > max_d)
                    max_d = accept_result;
            }
        }

    }
    
    return EXIT_SUCCESS;
}