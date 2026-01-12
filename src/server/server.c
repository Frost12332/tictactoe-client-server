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


#define DEFAULT_TCP 0
#define ALL_NETWORK "0.0.0.0"

#define TIMEOUT_SEC 5
#define TIMEOUT_NSEC 0



enum
{
    queue_connection = 5
};


typedef struct 
{
    int max_d;
    int socket_fd;

    struct timespec timeout;
    fd_set read_fds, write_fds;
}server_context_t;

static server_context_t server_context;



static int start_server();

static int initialize_listen_socket();


static void setup_timeout();

static void prepare_fd_sets();

static void prepare_server_fd_sets();

static void prepare_client_fd_sets();

static void handle_received_event();

static void handle_new_connection();




static int start_server()
{
    /*before start server need load server config*/

    /*maybe add another sub-modules like logging*/

    if (initialize_listen_socket() == -1)
        return -1;

    return 0;
}

static int initialize_listen_socket()
{
    int result;
    struct sockaddr_in addr;

    server_context.socket_fd = socket(AF_INET, SOCK_STREAM, 
                                      DEFAULT_TCP);
    server_context.max_d = server_context.socket_fd;

    if (server_context.socket_fd == -1)
        return -1;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ALL_NETWORK, &addr.sin_addr);
    addr.sin_port = htons(3000);

    result = bind(server_context.socket_fd, 
                  (struct sockaddr*)&addr,
                  sizeof(struct sockaddr_in));

    if (result == -1)
        return -1;

    result = listen(server_context.socket_fd, queue_connection);

    return result;
}


static void setup_timeout()
{
    server_context.timeout.tv_sec = TIMEOUT_SEC;
    server_context.timeout.tv_nsec = TIMEOUT_NSEC;
}

static void prepare_fd_sets()
{
    FD_ZERO(&server_context.read_fds);
    FD_ZERO(&server_context.write_fds);

    prepare_server_fd_sets();
    prepare_client_fd_sets();
}

static void prepare_server_fd_sets()
{
    FD_SET(server_context.socket_fd, &server_context.read_fds);
}

static void prepare_client_fd_sets()
{
    /*clients fd we receive as result accept function*/
    /*
    but for detect in which sets put fd need use some struct
    which help detect us which client read which speak
    */
}



static void handle_received_event()
{
    if (FD_ISSET(server_context.socket_fd, &server_context.read_fds))
    {
        int accept_result = accept(server_context.socket_fd, NULL, NULL);
                
        if (accept_result != -1)
        {
            printf("receive new connection\n");
            
            if (accept_result > server_context.max_d)
                server_context.max_d = accept_result;
        }
    }
}

static void handle_new_connection()
{
    
}




int main()
{
    int result;

    if (start_server())
        exit(EXIT_FAILURE);

    for(;;)
    {
        setup_timeout();
        prepare_fd_sets();


        result = select(server_context.max_d + 1, 
                        &server_context.read_fds,
                        &server_context.write_fds,
                        NULL, &server_context.timeout);

        if(result == 0)
            printf("Time out\n");
        else if (result > 0)
            handle_received_event();

    }
    
    return EXIT_SUCCESS;
}