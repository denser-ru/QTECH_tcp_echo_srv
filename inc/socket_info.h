//
// Created by denser on 02.02.2022.
//

#ifndef TCPECHOSRV_SOCKETINFO_H
# define TCPECHOSRV_SOCKETINFO_H

# include <string.h>
# include <arpa/inet.h>
# include <netinet/in.h>

typedef struct s_socket_info	t_socket_info;

struct s_socket_info
{
	char				cli_IP[ MAX_SRV_IP_STR_LEN + 1 ];
	unsigned int		cli_port;
	struct sockaddr_in	cli_addr;
	socklen_t			len;
};

void	socketinfo_init( t_socket_info *this );
void	socketinfo_get_addr_with_port( t_socket_info *info, int sock_fd, char *name );
int		itoa( int n, char s[] );

#endif //TCPECHOSRV_SOCKETINFO_H
