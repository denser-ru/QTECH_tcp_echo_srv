//
// Created by denser on 02.02.2022.
//

#include "server.h"

void	srv_respond_send( char *data )
{
	char			buf[ MAX_SRV_IP_STR_LEN + MAX_SRV_PORT_STR_LEN + 2 ];
	t_socket_info	info;

	send( g_srv->cli_sock_fd, data, strlen( data ), 0 );
	socketinfo_get_addr_with_port( &info, g_srv->cli_sock_fd, buf );
	printf( "\n---\nSent respond to client socket (%d), adrress %s\n---\n",
			g_srv->cli_sock_fd, buf);
	srv_client_cut(g_srv->cli_sock_fd);
	close(g_srv->cli_sock_fd);
}
