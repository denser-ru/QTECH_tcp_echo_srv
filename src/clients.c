//
// Created by denser on 03.02.2022.
//

# include "clients.h"

t_clients	*srv_client_new( void const *data, int fd, size_t data_size )
{
	t_clients		*client;

	client = ( t_clients* )malloc(sizeof( *client ) );
	bzero( client, sizeof( *client )) ;
	if ( !client )
		return ( NULL );
	if ( !data )
		client->data_size = 0;
	else
	{
		client->fd = fd;
		client->count = SRV_CLIENT_TIMEOUT;
		memcpy(client->data, data, data_size);
		client->data_size = data_size;
	}
	return ( client );
}

void	srv_clients_insert( int fd, void *data, size_t data_size )
{
	t_clients	**prev;
	t_clients	*cur;
	t_clients	*new;

	new = srv_client_new( data, fd, data_size );
	cur = g_srv->clients;
	while ( cur && cur->next && cur->fd < fd )
		cur = cur->next;
	prev = &cur;
	if (new != NULL)
	{
		if (*prev)
		{
			new->prev = *prev;
			new->next = (*prev)->next;
			(*prev)->next = new;
		}
		else
			*prev = new;
	}
}

t_clients	*srv_clients_pushb( int fd, void *data, size_t data_size )
{
	t_clients	**root;
	t_clients	*new;
	t_clients	*cur;

	root = &g_srv->clients;
	cur = *root;
	if ( *root == NULL )
		*root = srv_client_new( data, fd, data_size );
	else
	{
		new = srv_client_new( data, fd, data_size );
		if (!new)
			return (NULL);
		while (cur->next)
			cur = cur->next;
		cur->next = new;
		new->prev = cur;
	}
	return ( *root );
}

void	srv_client_delone(t_clients **alst, void (*del)( void *, size_t ) )
{
	if ( alst && (*alst) )
	{
		del( (*alst)->data, (*alst)->data_size );
		free( *alst );
		*alst = NULL;
	}
}

void	srv_clients_del( t_clients **alst, void (*del)( void *, size_t ) )
{
	if (*alst != NULL && del != NULL)
	{
		srv_clients_del( &((*alst)->next), del );
		srv_client_delone( alst, del );
	}
}

t_clients	*srv_client_find( int fd )
{
	t_clients *client;

	client = g_srv->clients;
	while ( client && client->fd != fd )
		client = client->next;

	return ( client );
}

void	srv_client_cut( int fd )
{
	t_clients	**root;
	t_clients	*cut;

	root = &g_srv->clients;
	cut = srv_client_find( fd );
	if ( !cut )
		return ;
	if ( cut == *root )
	{
		*root = cut->next;
		if ( *root )
			(*root)->prev = NULL;
		srv_client_delone( &cut, srv_client_delcontent );
	}
	else
	{
		if ( cut->next )
			cut->next->prev = cut->prev;
		cut->prev->next = cut->next;
		srv_client_delone( &cut, srv_client_delcontent );
	}
}

size_t	srv_clients_size( void )
{
	int			i;
	t_clients	*client;

	i = 0;
	client = g_srv->clients;
	while (client)
	{
		client = client->next;
		++i;
	}
	return ( i );
}

void	srv_client_delcontent( void *content, size_t size )
{
	( void )size;
	content += 0;
}
