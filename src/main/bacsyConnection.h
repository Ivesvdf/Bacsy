#ifndef BACSYCONNECTION_H
#define BACSYCONNECTION_H

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/DialogSocket.h"
#include "Poco/Net/TCPServerConnection.h" 

class BacsyConnection : public Poco::Net::TCPServerConnection
{
public:
	BacsyConnection(const Poco::Net::StreamSocket& socket);
	void storeBackup(Poco::Net::DialogSocket& ds);
	void backupFile(std::string file);
	virtual void run();
};

#endif
