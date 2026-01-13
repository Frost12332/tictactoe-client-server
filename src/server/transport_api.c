#include "transport_api.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

transport_context_t server_context;












static int initialize_listen_socket();



static void prepare_server_fd_sets();

static void prepare_client_fd_sets();



static void handle_new_connection();

static void handle_write_event();

static void handle_read_event();



static int is_receive_new_connection();

static int is_receive_write_event();

static int is_receive_read_event();

static int is_receive_event();



int start_server()
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


void setup_timeout()
{
    server_context.timeout.tv_sec = TIMEOUT_SEC;
    server_context.timeout.tv_nsec = TIMEOUT_NSEC;
}

void prepare_fd_sets()
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



void handle_received_event(int received_event)
{
    if (is_receive_new_connection())
    {
        handle_new_connection();
        received_event--;
    }
    if (received_event > 0)
    {
        if (is_receive_write_event())
            handle_write_event();
        if (is_receive_read_event())
            handle_read_event();
    }
}

static void handle_new_connection()
{
    int accept_result = accept(server_context.socket_fd, NULL, NULL);
            
    if (accept_result != -1)
    {
        printf("receive new connection\n");
        
        if (accept_result > server_context.max_d)
            server_context.max_d = accept_result;
    }
}

static void handle_write_event()
{

}

static void handle_read_event()
{

}


static int is_receive_new_connection()
{
    return FD_ISSET(server_context.socket_fd, &server_context.read_fds);
}

static int is_receive_write_event()
{
    return is_receive_event();
}

static int is_receive_read_event()
{
    return is_receive_event();
}

static int is_receive_event()
{
    return 0;
}