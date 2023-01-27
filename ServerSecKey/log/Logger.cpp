#include "Logger.h"
#include <iostream>

Logger Logger::m_log;

Logger::Logger()
{
	m_writer.open("serverSeckey.log");
}

Logger* Logger::getInstance()
{
	return &m_log;
}

void Logger::Log(std::string text, std::string file, int line, Level level)
{
	bool flag = level >= m_level;
	if (flag)
	{
		if (m_device == CONSOLE)
		{
			// 日志信息控制台输出
			std::cout << "File[ " << file << " ], Line[ " << line
				<< " ], Infomation[ " << text << " ]" << std::endl;
		}
		else
		{
			// 日志信息写文件
			m_writer << "File[ " << file << " ], Line[ " << line
				<< " ], Infomation[ " << text << " ]" << std::endl;
			// 数据强制刷到磁盘
			m_writer.flush();
		}

	}
}

void Logger::setEnableLevel(Level level)
{
	m_level = level;
}

Logger::~Logger()
{
	m_writer.close();
}
