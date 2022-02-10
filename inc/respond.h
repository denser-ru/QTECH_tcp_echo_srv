//
// Created by denser on 02.02.2022.
//

#ifndef TCPECHOSRV_RESPOND_H
# define TCPECHOSRV_RESPOND_H

# include <unistd.h>
# include <sys/socket.h>
# include <time.h>
# include "server.h"
# include "socket_info.h"

# include "server.h"

void	srv_respond_send( char *data );

void error(const char *msg);

#endif //TCPECHOSRV_RESPOND_H
