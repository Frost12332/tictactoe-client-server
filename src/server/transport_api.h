#ifndef TRANSPORT_API_H
#define TRANSPORT_API_H
/*
subscrive on event for receive callback
start or stop for de/initialize
and await event for receive event
*/

typedef enum
{
    read_event,
    write_event
}received_event_type_t;

typedef enum
{
    on_init,
    on_shut,

    on_setup_fd,

    on_accept,
    on_read,
    on_write
}transport_events_name_t;


void subscribe_on_event(void (*function_pointer) (void), transport_events_name_t event_name);


int start_transport_level();

void stop_transport_level();


int await_event();


int accept_new_connection();

int is_receive_new_connection();


int is_receive_event(int fd, received_event_type_t event_type);

int handle_received_event(int fd, received_event_type_t event_type, char* buffer, int length);

#endif