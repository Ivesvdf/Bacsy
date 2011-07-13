#ifndef BACSYSERVER_H
#define BACSYSERVER_H

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "bacsyConnection.h"

using Poco::Net::TCPServerConnectionFactoryImpl;

class BacsyServer : public Poco::Net::TCPServer
{
public:
	BacsyServer();
};

#endif
