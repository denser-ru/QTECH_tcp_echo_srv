//
// Created by denser on 26.01.2022.
//

#include "tcp_echo_srv.h"
#include "socket_info.h"

void	SocketInfo_init( t_socket_info *info )
{
	bzero(&info->cli_addr, sizeof(info->cli_addr));
	info->len = sizeof(info->cli_addr);
}

void	socketinfo_get_addr_with_port(  t_socket_info *info, int sock_fd,
										char *name )
{
	char 	buf_i[ MAX_SRV_PORT_STR_LEN + 1 ];
	size_t	len;

	bzero( name, sizeof( *name) );
	bzero( buf_i, sizeof( buf_i) );
	getsockname( sock_fd, ( struct sockaddr* ) &info->cli_addr, &info->len );
	inet_ntop( AF_INET, &info->cli_addr.sin_addr, info->cli_IP,
			   sizeof( info->cli_IP ) );
	info->cli_port = ntohs(info->cli_addr.sin_port);
	strncpy( name, info->cli_IP, MAX_SRV_IP_STR_LEN);
	len = strlen( name );
	if ( len < REQ_BUFFER_SIZE && len
			+ itoa( ( int )info->cli_port, buf_i ) < REQ_BUFFER_SIZE )
	{
		name[ len++ ] = ':';
		strncpy( name + len, buf_i, 11 );
	}
}