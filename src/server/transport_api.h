#ifndef TRANSPORT_API_H
#define TRANSPORT_API_H


#include <time.h>
#include <sys/select.h>

#define DEFAULT_TCP 0
#define ALL_NETWORK "0.0.0.0"

#define TIMEOUT_SEC 5
#define TIMEOUT_NSEC 0


enum
{
    queue_connection = 5
};

typedef enum
{
    on_init,
    on_shut,

    on_setup_fd,

    on_accept,
    on_read,
    on_write
}transport_events_name_t;

typedef struct 
{
    int max_d;
    int socket_fd;

    struct timespec timeout;
    fd_set read_fds, write_fds;
}transport_context_t;

extern transport_context_t server_context;


void subscribe_on_event(void (*function_pointer) (void), transport_events_name_t event_name);



int start_server();

void setup_timeout();

void prepare_fd_sets();

void handle_received_event();

#endif