#pragma once
#include <string>
#include <fstream>
// 饿汉模式单例类
class Logger
{
public:
	enum Type { CONSOLE, FILE };
	enum Level { DEBUG, INFO, WARNING, ERROR, CRITICAL };
	static Logger* getInstance();
	void Log(std::string text, std::string file, int line, Level level = INFO);
	void setEnableLevel(Level level);
	inline void setDevice(Type device)
	{
		m_device = device;
	}
	~Logger();
private:
	Logger();
	Logger(const Logger& log);

private:
	std::ofstream m_writer;
	static Logger m_log;
	Level m_level = INFO;
	Type m_device = FILE;
};

