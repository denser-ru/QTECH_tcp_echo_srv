//
// Created by denser on 02.02.2022.
//

#ifndef TCPECHOSRV_SIGNALCATCH_H
# define TCPECHOSRV_SIGNALCATCH_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>

# include "tcp_echo_srv.h"

extern int			signal_catch_run;

typedef struct s_srv_signal	t_srv_signal;

struct s_srv_signal
{
	struct sigaction	sa;
};

void	signalcatch_start( t_srv_signal *this );
void 	term_handler(int i);

#endif //TCPECHOSRV_SIGNALCATCH_H
