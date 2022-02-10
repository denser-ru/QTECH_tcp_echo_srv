//
// Created by denser on 02.02.2022.
//

#include "server.h"
#include "signal_catch.h"

t_server	*g_srv = NULL;
int			signal_catch_run = 1;

char	*srv_getdata( char *data );
void	srv_setconfig( t_srv_cfg *cfg);

int main()
{
	char			data[ SRV_BUFFER_SIZE + 1 ]; // контейнер данных для ответа клиенту
	g_srv 			= new_server(); // инициализация Серевера
	t_srv_signal	srv_signal;
	t_srv_cfg		config; // структура настроек сервера

	srv_setconfig( &config ); // заполняем настройки
	signalcatch_start( &srv_signal);
	srv_start( &config ); // запуск сервера
	while (1)
	{
		srv_request_get(); // получение запроса клиента из очереди
		srv_getdata( data ); // формирование ответа клиенту
		srv_respond_send( data ); // отправка ответа
	}

	free( g_srv );
	return 0;
}

char	*srv_getdata( char *data )
{
	data = "HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nwebserver\r\n";
	return (data);
}


void	srv_setconfig( t_srv_cfg *cfg)
{
	bzero((char *)cfg, sizeof( *cfg ));
	cfg->serv_addr.sin_family = AF_INET;
//	cfg->serv_addr.sin_addr.s_addr = INADDR_ANY;
	cfg->serv_addr.sin_addr.s_addr = inet_addr( "192.168.1.8" );
	cfg->serv_addr.sin_port = htons( SRV_PORT );
}
