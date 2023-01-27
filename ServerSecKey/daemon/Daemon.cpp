#include "Daemon.h"



int Daemon::daemonProcess()
{
	pid_t  pid = fork();
	int ret = 0;

	if (pid > 0)
	{
		exit(0);
	}
	ret = _setsid();
	if (ret == -1)
	{
		perror("setsid");
		return -1;
	}

	ret = _setlocalDir();
	if (ret == -1)
	{
		perror("chdir");
		return -1;
	}

	_umask();

	return ret;
}

int Daemon::_setsid()
{
	int ret = setsid();
	if (ret < 0)
	{
		return -1;
	}
	return 0;
}

int Daemon::_setlocalDir()
{

	int ret = chdir(this->m_path);
	if (ret < 0)
	{
		return -1;
	}
	return 0;
}

void Daemon::_umask()
{
	umask(0);
}

Daemon::~Daemon()
{

}