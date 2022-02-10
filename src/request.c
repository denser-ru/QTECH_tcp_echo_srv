//
// Created by denser on 02.02.2022.
//

#include "server.h"

void	srv_request_get( void )
{
	bzero(g_srv->request.buf, sizeof(g_srv->request));
	get_next_request_part();
}

void	get_next_request_part( void )
{
	int bytes_read;

	while (1)
	{
		fd_set readset;
		struct timeval timeout;

//		readset = NULL;
		// Заполняем множество сокетов
		filling_a_lot_of_sockets( &readset, &timeout );

		// Ждём события в одном из сокетов
		waiting_for_events( &readset, &timeout);

		// Определяем тип события и выполняем соответствующие действия
		determine_event_and_perform( &readset );

		//Проверка получения новых данных
		for (t_clients *it = g_srv->clients; it; it = it->next)
		{
			if (FD_ISSET( it->fd, &readset ))
			{
				// Поступили данные от клиента, читаем их
				if ( received_data_client( &bytes_read, it ) <= 0 )
					break;

				g_srv->cli_sock_fd = it->fd;
				if (it->data_size + bytes_read <= REQ_BUFFER_SIZE)
				{
					strncpy(it->data + bytes_read, g_srv->request.buf, bytes_read);
					it->data_size += bytes_read;
					// Если получен двойной перенос строки, завершаем получение запроса
					if (check_end_request(it))
						return;
				}
				bzero(g_srv->request.buf, bytes_read);
			}
		}
	}
}

void	filling_a_lot_of_sockets( fd_set *readset, struct timeval *timeout )
{
	// Заполняем множество сокетов
	FD_ZERO(readset);
	FD_SET(g_srv->cfg.sockfd, readset);

	for (t_clients *it = g_srv->clients; it; it = it->next)
		FD_SET(it->fd, readset);

	// Задаём таймаут
	timeout->tv_sec = SRV_SELECT_TIMEOUT;
	timeout->tv_usec = 0;
}

void	waiting_for_events( fd_set *readset, struct timeval *timeout )
{
	char			buf[ MAX_SRV_IP_STR_LEN + MAX_SRV_PORT_STR_LEN + 2 ];
	t_socket_info	info;

	// Ждём события в одном из сокетов
	int mx = -1;
	if ( g_srv->clients )
		mx = g_srv->clients->fd;
	{
		int t = select( mx + 1, readset, NULL, NULL, timeout );
		if ( t < 0 )
		{
			perror("select");
			exit(3);
		}
		else if (t == 0)
		{
			printf("waiting requests from (%zu) clients ...\n",
				   srv_clients_size() );
//			request_close_client_connections();
		}
		else
		{
			socketinfo_get_addr_with_port( &info, mx, buf );
			printf( "received part of a request from client socket (%d)"
					", adrress %s\n", mx, buf);
			srv_client_find( mx )->count = SRV_CLIENT_TIMEOUT;
		}
	}
}

void	determine_event_and_perform( fd_set *readset )
{
	// Определяем тип события и выполняем соответствующие действия
	if (FD_ISSET(g_srv->cfg.sockfd, readset))
	{
		// Поступил новый запрос на соединение, используем accept
		g_srv->cli_sock_fd = accept(g_srv->cfg.sockfd, NULL, NULL);
		if (g_srv->cli_sock_fd < 0)
		{
			perror("accept");
			exit(3);
		}

		fcntl(g_srv->cli_sock_fd, F_SETFL, O_NONBLOCK);

		srv_clients_pushb( g_srv->cli_sock_fd, "", 0 );
	}
}

int	received_data_client( int *bytes_read, t_clients *client )
{
	// Поступили данные от клиента, читаем их
	*bytes_read = recv(client->fd, g_srv->request.buf, REQ_BUFFER_SIZE, 0);

	if (*bytes_read <= 0)
	{
		// Соединение разорвано, удаляем сокет из множества
		close(client->fd );
		printf( "Connection on socket (%d) dropped\n", client->fd );
		srv_client_cut( client->fd );
	}
	return (*bytes_read);
}

int	check_end_request( t_clients *client )
{
	if ( strstr( ( char* )client->data, "\r\n\r\n" ) )
	{
		printf("\n+++\nReceived from socket (%d): %s\n+++\n", client->fd, ( char* )client->data);
		strncpy( g_srv->request.data, client->data, client->data_size );
		return (1);
	}
	return (0);
}

void	check_timeout( fd_set *readset )
{
	t_clients	*client;

	client = g_srv->clients;
	while ( client )
	{
		--client->count;
		client = client->next;
	}
	client = g_srv->clients;
	while ( client )
	{
		if (client->count < 1)
		{
			printf("Connection (%d) dropped\n", client->fd);
			close(client->fd);
			FD_CLR(client->fd, readset);
			srv_client_cut(client->fd);
			client = g_srv->clients;
			continue;
		}
		client = client->next;
	}
}

void	request_close_client_connections( void )
{
	for ( t_clients *it = g_srv->clients; it; it = it->next )
	{
		printf( "Connection (%d) dropped\n", it->fd );
		close( it->fd );
	}
	srv_clients_del( &g_srv->clients, srv_client_delcontent );
}
