#include <cstdio>
#include "./tcpserver/ServerOP.h"
#include "./daemon/Daemon.h"



int main()
{

	ServerOP serOP("server.json");
	Daemon daemonPro;
	Logger* log = Logger::getInstance();
	int err = daemonPro.daemonProcess();
	if (err == -1)
	{
		log->Log("daemon err...", __FILE__, __LINE__);
		//cout << "daemon err..\n";
		return 0;
	}
	serOP.startServer();

	return 0;
}
