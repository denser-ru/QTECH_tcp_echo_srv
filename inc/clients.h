//
// Created by denser on 03.02.2022.
//

#ifndef TCP_ECHO_SRV_CLIENTS_H
# define TCP_ECHO_SRV_CLIENTS_H

# include "server.h"

typedef struct s_clients	t_clients;

struct s_clients
{
	int					fd;
	size_t				count;
	char				data[ REQ_BUFFER_SIZE + 1 ];
	size_t				data_size;
	struct s_clients	*prev;
	struct s_clients	*next;
};

t_clients	*srv_client_new( void const *data, int fd, size_t data_size );
void		srv_clients_insert( int fd, void *data, size_t data_size );
t_clients	*srv_clients_pushb( int fd, void *data, size_t data_size );
void		srv_client_delone( t_clients **alst, void (*del)(void *, size_t) );
void		srv_clients_del( t_clients **alst, void (*del)(void *, size_t) );
t_clients	*srv_client_find( int fd );
void		srv_client_cut( int fd );
size_t		srv_clients_size ( void );
void		srv_client_delcontent( void *content, size_t size );

#endif //TCP_ECHO_SRV_CLIENTS_H
