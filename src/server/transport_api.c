#include "transport_api.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>


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
}transport_context_t;

typedef struct
{
    void (*on_initialize) (void);
    void (*on_shutdown) (void);

    void (*on_setup_fd_set) (void);
    
    void (*on_accept_connection) (void);
    void (*on_readable) (void);
    void (*on_writable) (void);

}transport_level_events_t;



static transport_context_t server_context;
static transport_level_events_t transport_level_events;


static int initialize_listen_socket();

static void setup_timeout();

static void prepare_fd_sets();

static void prepare_server_fd_sets();

static int call_select();



/*maybe use lists for multiple callbacks, check this later*/
void subscribe_on_event(void (*function_pointer) (void), transport_events_name_t event_name)
{
    switch (event_name)
    {
    case on_init:
        transport_level_events.on_initialize = function_pointer;
        break;
    case on_shut:
        transport_level_events.on_shutdown = function_pointer;
        break;
    case on_setup_fd:
        transport_level_events.on_setup_fd_set = function_pointer;
        break;
    case on_accept:
        transport_level_events.on_accept_connection = function_pointer;
        break;
    case on_read:
        transport_level_events.on_readable = function_pointer;
        break;
    case on_write:
        transport_level_events.on_writable = function_pointer;
        break;
    }
}


int start_transport_level()
{
    /*before start server need load server config*/

    /*maybe add another sub-modules like logging*/
    if (transport_level_events.on_initialize != NULL)
        transport_level_events.on_initialize();


    if (initialize_listen_socket() == -1)
        return -1;

    return 0;
}


void stop_transport_level()
{
    perror("stop_transport_level not implemented");
}


int await_event()
{
    setup_timeout();
    prepare_fd_sets();
    
    return call_select();
}


int accept_new_connection()
{
    int accept_result = accept(server_context.socket_fd, NULL, NULL);
            
    if (accept_result == -1)
        perror("receive new connection\n");

    if (accept_result > server_context.max_d)
            server_context.max_d = accept_result;

    return accept_result;
}

int is_receive_new_connection()
{
    return FD_ISSET(server_context.socket_fd, &server_context.read_fds);
}


int is_receive_event(int fd, received_event_type_t event_type)
{
    if(event_type == read_event)
    {
        return FD_ISSET(fd, &server_context.read_fds);
    }
    else
    {
        return FD_ISSET(fd, &server_context.write_fds);
    }
    return 0;
}

int handle_received_event(int fd, received_event_type_t event_type, char* buffer, int length)
{
    if (event_type == read_event)
        return read(fd, buffer, length);
    else
        return write(fd, buffer, length);
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

    transport_level_events.on_setup_fd_set();
}

static void prepare_server_fd_sets()
{
    FD_SET(server_context.socket_fd, &server_context.read_fds);
}

static int call_select()
{
    return pselect(server_context.max_d + 1, 
                        &server_context.read_fds,
                        &server_context.write_fds,
                        NULL, &server_context.timeout, NULL);
}