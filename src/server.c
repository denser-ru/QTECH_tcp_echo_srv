//
// Created by denser on 02.02.2022.
//

#include "server.h"
#include <stddef.h>

void error(const char *msg);

t_server		*new_server( void )
{
	g_srv = ( t_server* )malloc( sizeof( t_server ) );
	bzero((char *)&g_srv->cfg, sizeof( g_srv->cfg ) );
//	g_srv->cli_sock_fd = 0;
//	g_srv->clients = NULL;
	return ( g_srv );
}

void	srv_configure( t_srv_cfg *config )
{
	memcpy((char *)&g_srv->cfg, ( char* )config, sizeof( t_srv_cfg ) );
}

int		srv_start( t_srv_cfg *config )
{
	printf( "Webserv started pid: %d\n", getpid());
	srv_configure( config );
	g_srv->cfg.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	srv_net_up();
	printf( "Socket opening ( %d ) ... ", g_srv	->cfg.sockfd );
	if ( g_srv	->cfg.sockfd < 0 )
		error("ERROR");
	else
		printf( "OK\n" );
	printf( "Binding addr %s"
			":%d ... ", inet_ntoa(g_srv->cfg.serv_addr.sin_addr),
				htons(g_srv->cfg.serv_addr.sin_port));
	if (bind(g_srv->cfg.sockfd, (struct sockaddr *) &g_srv->cfg.serv_addr,
			 sizeof(g_srv->cfg.serv_addr)) < 0)
		error("ERROR");
	else
		printf( "OK\n" );
	listen(g_srv->cfg.sockfd,MAX_CON);
	return (0);
}

void	srv_init(void )
{
	printf( "Socket (%d) ", g_srv	->cfg.sockfd );
	close(g_srv->cfg.sockfd);
	printf( "dropped\n"
			"Webserv in addr %s:%d stoped\n",
				inet_ntoa(g_srv->cfg.serv_addr.sin_addr),
				htons(g_srv->cfg.serv_addr.sin_port));
}

void	error(const char *msg)
{
	printf( "%s\n", msg );
	exit(1);
}

int		srv_net_up( void )
{
	char const			devname[] = "wlp1s0:1";
	struct sockaddr_in	*in_addr;

	snprintf(g_srv->cfg.ifr.ifr_name, IFNAMSIZ, devname);
	if (ioctl(g_srv->cfg.sockfd, SIOCGIFFLAGS, &g_srv->cfg.ifr) < 0)
		printf("устройство \"%s\" НЕ обнаружено, ошибка: %s\n", devname, strerror(errno));
	else
		printf("устройство \"%s\" обнаружено\n", devname);

	g_srv->cfg.ifr.ifr_flags |= IFF_UP;
	if (ioctl(g_srv->cfg.sockfd, SIOCSIFFLAGS, &g_srv->cfg.ifr) < 0) {
		printf("Не получить доступ к %s, ошибка: %s\n", devname, strerror(errno));
	}

	in_addr = (struct sockaddr_in *)&g_srv->cfg.ifr.ifr_addr;
	in_addr->sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.1.7", &(in_addr->sin_addr));

	memcpy(&g_srv->cfg.ifr.ifr_addr, in_addr, sizeof(struct sockaddr));

	if (ioctl(g_srv->cfg.sockfd, SIOCSIFADDR, &g_srv->cfg.ifr) < 0) {
		printf("Не установить IP адрес для %s, ошибка: %s\n", g_srv->cfg.ifr.ifr_name, strerror(errno));
	}

	if (ioctl(g_srv->cfg.sockfd, SIOCGIFADDR, &g_srv->cfg.ifr) < 0) {
		printf("Не получить IP адрес для %s, ошибка: %s\n", g_srv->cfg.ifr.ifr_name, strerror(errno));
	}
	printf("Интерфейс %s IP адрес: %s\n", devname, inet_ntoa(in_addr->sin_addr));


	return 0;

}