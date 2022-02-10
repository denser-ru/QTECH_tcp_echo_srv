//
// Created by denser on 02.02.2022.
//

#ifndef TCPECHOSRV_CORE_H
# define TCPECHOSRV_CORE_H

# include <stdlib.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/ioctl.h>
# include <errno.h>
#include <net/if.h>


# include "tcp_echo_srv.h"
# include "request.h"
# include "respond.h"
# include "clients.h"

typedef	struct s_server		t_server;
typedef	struct s_srv_cfg	t_srv_cfg;
typedef struct s_clients	t_clients;
typedef struct s_request	t_request;
//typedef struct s_respond	t_respond;

extern t_server				*g_srv;

struct	s_srv_cfg
{
	int							sockfd;
	struct ifreq				ifr;
	struct sockaddr_in			serv_addr;
//	struct sockaddr_in			cli_addr;
};

struct	s_server
{
	t_srv_cfg		cfg;
	int				cli_sock_fd;
	t_clients		*clients;

	t_request			request;
//	t_respond			respond;
};

t_server	*new_server( void );
int			srv_start( t_srv_cfg *config );
void		srv_configure( t_srv_cfg *config );
int			srv_net_up( void );

//init( void );
//free( void );

#endif //TCPECHOSRV_CORE_H
