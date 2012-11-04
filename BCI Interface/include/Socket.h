// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>

const int MAXRECV = 500;

class Socket
{
public:
	Socket();
	virtual ~Socket();

    void close();

	bool create();
	bool connect ( const std::string host, const int port );
	bool bind (const std::string host, const int port);

	// Data Transimission
	bool send ( const std::string ) const;
	int recv ( std::string& ) const;

	void set_non_blocking ( const bool );

	bool is_valid() const { return m_sock != -1; }

private:

	int m_sock;
	sockaddr_in m_addr;
};

#endif
