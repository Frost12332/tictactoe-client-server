#ifndef SESSION_API_H
#define SESSION_API_H

#include "transport_api.h"
#include "../shared/buffer.h"


typedef enum
{
    read_message,
    write_message
}session_state_t;


typedef struct
{
    int fd;
    
    session_state_t session_state;
    
    buffer_t* send_buffer;
    buffer_t* receive_buffer;

}session_t;


int start_session_level();

void stop_session_level();








#endif