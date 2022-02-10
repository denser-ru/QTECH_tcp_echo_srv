//
// Created by denser on 24.01.2022.
//

#include "signal_catch.h"
#include "server.h"

void	signalcatch_start( t_srv_signal *this )
{
	sigset_t newset;

	sigemptyset(&newset);   // чистимся
	sigaddset(&newset, SIGHUP);  // добавляем сигнал SIGHUP
	sigaddset(&newset, SIGABRT);  // добавляем сигнал SIGABRT
	sigprocmask(SIG_BLOCK, &newset, 0);   // блокируем его
	this->sa.sa_handler = term_handler;  // указываем обработчик
	sigaction(SIGTERM, &this->sa, 0);    // обрабатываем сигнал SIGTERM
	sigaction(SIGABRT, &this->sa, 0);    // обрабатываем сигнал SIGABRT
	sigaction(SIGINT, &this->sa, 0);    // обрабатываем сигнал SIGINT
}

void term_handler(int i)
{
	printf( "\nSignal received \"%s\" (%d)\n",strsignal( i ), i );
//	srvFree();
	exit(EXIT_SUCCESS);
}