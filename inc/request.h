//
// Created by denser on 02.02.2022.
//

#ifndef TCPECHOSRV_REQUEST_H
# define TCPECHOSRV_REQUEST_H

# include "server.h"

# include <sys/types.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <fcntl.h>

typedef struct s_request	t_request;
typedef struct s_clients	t_clients;

struct	s_request
{
	char	buf[ REQ_BUFFER_SIZE + 1 ];
	char	data[ REQ_BUFFER_SIZE + 1 ];
};

void	error( const char *msg );
void	srv_request_get( void );
void	check_timeout( fd_set *readset );
void	request_close_client_connections( void );

void	get_next_request_part( void );
void	filling_a_lot_of_sockets( fd_set *readset, struct timeval *timeout );
void	waiting_for_events( fd_set *readset, struct timeval *timeout );
void	determine_event_and_perform( fd_set *readset );
int		received_data_client( int *bytes_read, t_clients *client );
int		check_end_request( t_clients *client );

#endif //TCPECHOSRV_REQUEST_H
