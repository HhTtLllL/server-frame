//##################################################################
// File Name: sylar/log.cpp
// File Effect: 
// Author: tttt
// mail: tttt@xiyoulinux.org
// Created Time: 2020年07月29日 星期三 16时05分32秒
//=============================================================
//!/usr/bin/python

#include "log.h"

namespace sylar
{

Logger(const std::string& name) : m_name(name)
{

}

void addAppender ( LogAppender::ptr appender )
{
	m_appenders.push_back(appender);

}
void delAppender ( LogAppender::ptr appender )
{
	for( auto it = m_appenders.begin() ;
			it != m_appenders.end(); it ++; )
	{
		if( *it == appender )
		{
			m_appenders.erease(it);
			break;
		}
	}

}

void Logger::log( LogLevel::Level  level,LogEvent::ptr event)
{
	if( level >= m_level )
	{
		for( auto& i : m_appenders )
		{
			i->log( level,event);
		}
	}
}
void Logger::debug(LogEvent::ptr event)
{
	debug(LogLevel::DEBUG,event);

}
void Logger::info(LogEvent::ptr event)
{
	debug(LogLevel::INFO,event);

}
void Logger::warn(LogEvent::ptr event)
{
	debug(LogLevel::WARN,event);

}
void Logger::error(LogEvent::ptr event)
{
	debug(LogLevel::ERROR,event);


}
void Logger::fatal(LogEvent::ptr event)
{
	debug(LogLevel::FATAL,event);


}

}

FileLogAppender::FileLogAppender ( const std::string& filename) : m_filename(filename)
{

}

bool  FileLogAppender::log( LogLevel::Level level,LogEvent::ptr event)
{
	if( level >= m_level)
	{
		m_filestream << m_formatter.format(event);
	}

}

void FileLogAppender::reopen()
{
	if( m_filestream)
	{
		m_filestream.close();
	}

	m_filestream.open(m_filename);


	return !!m_filestream;
}

void StdoutLogAppender::log( LogLevel::Level level,LogEvent::ptr event)
{
	if(level >= m_level)
	{
		std::cout << m_formatter.format(event);
	}

}
