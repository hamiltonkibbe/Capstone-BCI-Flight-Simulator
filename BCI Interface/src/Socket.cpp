// Implementation of the Socket class.


#include <string.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "Socket.h"

using namespace std;

Socket::Socket() :  m_sock ( -1 ) {
	memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::~Socket() {
	if ( is_valid() )
		::close ( m_sock );
}

void Socket::close()
{
    if ( is_valid() )
        ::close(m_sock);
}

bool Socket::create() {
	m_sock = socket ( AF_INET, SOCK_DGRAM, 0 );

	if ( ! is_valid() )
		return false;

	int on = 1;
	if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
		return false;

	return true;
}

bool Socket::send ( const std::string s ) const {
	int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
	if ( status == -1 )	{
		cout << "Error sending: send_status = " << status << endl;
		return false;
	} else {
		//cout << "send_status = " << status << endl;
		return true;
	}
}

int Socket::recv ( std::string& s ) const {
	char buf [ MAXRECV + 1 ];

	s = "";

	memset ( buf, 0, MAXRECV + 1 );

	int status = ::recv ( m_sock, buf, MAXRECV, 0 );

	if ( status == -1 )	{
		cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
		return 0;
	} else if ( status == 0 ) {
		cout << "recv() returned 0, FlightGear closed connection\n";
		return 0;
	} else {
		//cout << "bytes received = " << status << "\n";
		s = buf;
		return status;
	}
}

bool Socket::connect ( const std::string host, const int port ) {
	if ( ! is_valid() ) return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

	if ( errno == EAFNOSUPPORT ) return false;

	status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( status == 0 ) {
		return true;
	} else {
		return false;
	}
}

bool Socket::bind ( const std::string host, const int port )
{
	if ( ! is_valid() ) {
		return false;
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(host.c_str());
	m_addr.sin_port = htons ( port );

	int bind_return = ::bind ( m_sock,
			( struct sockaddr * ) &m_addr,
			sizeof ( m_addr ) );


	if ( bind_return == -1 ) {
		return false;
	}

	return true;
}

void Socket::set_non_blocking ( const bool b ) {
	int opts;

	opts = fcntl ( m_sock, F_GETFL );

	if ( opts < 0 )	{
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_sock,	F_SETFL, opts );
}
