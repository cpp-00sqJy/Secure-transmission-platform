#pragma once

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>


class Daemon
{
public:
	Daemon(const char* path = "/") :m_path(path) { };
	~Daemon();

	int daemonProcess();

private:

	int _setsid();

	int _setlocalDir();

	void _umask();

private:

	const char* m_path;

};

