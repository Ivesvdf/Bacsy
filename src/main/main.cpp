#include "bacsyServer.h"
#include "Poco/Thread.h"


int main()
{
	BacsyServer server;
	server.start();

	while(true)
		Poco::Thread::sleep(10000);
}
