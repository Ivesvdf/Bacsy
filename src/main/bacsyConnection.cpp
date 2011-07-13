#include "woodcutter/woodcutter.h"
#include "bacsyConnection.h"
#include "stringUtils.h"
#include "info.h"


BacsyConnection::BacsyConnection(const Poco::Net::StreamSocket& socket) : Poco::Net::TCPServerConnection(socket)
{

}

void BacsyConnection::run()
{
	LOGI("New client connected -- starting BacsyConnection");

	Poco::Net::DialogSocket ds(socket());

	std::string magicString;
	ds.receiveMessage(magicString);

	if(magicString != "BACSY " + bacsyVersion)
	{
		LOGE("Invalid magic string; do protocol versions match? -- stopping connection");
		return;
	}

	std::string command;
	ds.receiveMessage(command);

	if(command == "STORE")
	{
		storeBackup(ds);
	}
	else
	{
		LOGE("Unrecognized command: " + command + " -- stopping connection");
		return;
	}
	LOGI("Naturally stopping connection with client");
}


void BacsyConnection::storeBackup(Poco::Net::DialogSocket& ds)
{
	LOGI("Storing backup.");

	std::string hostIdentification;
	ds.receiveMessage(hostIdentification);

	LOGI("Backup is for " + hostIdentification);

	// Test if there are any stores available

	std::string file;
	std::string checksum;
	while(ds.receiveMessage(file) && ds.receiveMessage(checksum))
	{
		backupFile(file);
	}
}

void BacsyConnection::backupFile(std::string file)
{
	LOGI("Pushing file " + file + " to the backup queue.");
}

