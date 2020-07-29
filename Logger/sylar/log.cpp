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

const char* LogLevel::ToString(LogLevel::Level level)
{
	switch(level)
	{
#define xx( name) \
		case LogLevel::name:\
				    return #name;\
		break;


		xx(DEBUG);
		xx(INFO);
		xx(WARN);
		xx(ERROR);
		xx(FATAL);
#undef xx 
		default:
			return  "UNKNOW";
	}

	return "UNKNOW";
}

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

LogFormatter::LogFormatter( const std::string& pattern) : m_pattern(pattern)
{

}

std::string LogFormatter::format(LogLevel::Level level , LogEvent::ptr event)
{
	std::stringstream ss;
	for( auto& i : m_items)
	{
		i->format(ss,level,event);
	}

	return ss.str();
}

//%xxx  %xxx{xxx}  %%
void LogFormatter::init()
{
	//str,format,type
	std::vector<std::tuple<std::string,std::string,int> > vec;
	std::string nstr;

	for( size_t i = 0; i < m_pattern.size(); ++i)
	{
		if( m_pattern[i] != '%' )
		{
			nstr.append(1,m_pattern[i]);
			continue;
		}

		if(( i + 1 ) < m_pattern.size() )
		{
			if( m_pattern[i + 1] == '%')
			{
				nstr.append(1,'%');
				continue;
			}
		}

		size_t n = i+ 1;
		int fmt_status = 0;
		size_t fmt_begin = 0;

		std::string str;
		std::string fmt;

		while( n < m_pattern.size() )
		{
			if(isspace(m_pattern[n])) break;

			if( fmt_status == 0)
			{
				if( m_pattern[n] == '{')
				{
					str = m_pattern.substr(i + 1, n - i - 1);
					fmt_status = 1; //解析格式
					fmt_begin = n;
					++n;
					continue;
				}
			}

			if(fmt_status == 1) 
			{
				if(m_pattern[n] == '}')
				{
					fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
					fmt_status = 2;
					break;
				}
			}
		}

		if( fmt_status == 0)
		{
			if(!nstr.empty())
			{
				vec.push_back(std::makepair(nstr,"",0));
			}

			str = m_pattern.substr(i + 1, n - i - 1);
			vec.push_back(std::make_tuple(str,fmt,1));
			i = n;
		}
		else if( fmt_status == 1)
		{
			std::cout << "pattern parse error :" << m_pattern << " - " << m_pattern.substr(i) << std::endl;
		}
		else if(fmt_status == 2)
		{
			if( !nstr.empty())
			{
				vec.push_back(std::make_pair(nstr,"",0));
			}

			vec.push_back(std::make_tuple(str,fmt,1));
			i = n;
		}
	}


	if( !nstr.empty())
	{
		vec.push_back(std::make_pair(nstr,"",0));
	}
}


class MessageFormatItem : public LogFormatter::FormatItem
{
	public:
		void format( std::ostream& os,LogLevel::Level level, LogEvent::ptr event) override
		{
			os << event->getContent();
		}
};

class LevelFormatItem : public LogFormatter::FormatItem
{
	public:
		void format(std::ostream& os,LogLevel::Level level , LogEvent::ptr event) override
		{
			os << LogLevel::ToString(level);
		}
};

       const char * m_file = nullptr;  //文件名
       int32_t m_line = 0;         //行号
       uint32_t m_elapse = 0;  //程序启动开始到现在的毫秒数
       uint32_t m_threadId = 0;     //线程 id
       uint32_t m_fiberId = 0;       //协程  id
       uint64_t m_time;             //时戳
       std::string m_content;        //消息






}
