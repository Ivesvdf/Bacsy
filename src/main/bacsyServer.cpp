#include "bacsyServer.h"

BacsyServer::BacsyServer(): 
	Poco::Net::TCPServer(
			new TCPServerConnectionFactoryImpl<BacsyConnection>(),
			Poco::Net::ServerSocket(26951),
			0)
{

}
