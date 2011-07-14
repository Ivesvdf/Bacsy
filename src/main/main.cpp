#include <fstream>
#include "Poco/Thread.h"
#include "bacsyServer.h"
#include "target.h"
#include "configurationFile.h"



int main()
{
	BacsyServer server;
	server.start();

	std::ifstream filestream(".bacsy/targets.config");
	Target target("basicTarget", ConfigurationFile(filestream));
	target.start();
	
	while(true)
		Poco::Thread::sleep(10000);
}
