#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>



namespace sylar
{


	//日志事件
class LogEvent
{
	public:
		typedef std::shared_ptr<LogEvent> ptr;
		LogEvent( );
	private:
		const char * m_file = nullptr;  //文件名
		int32_t m_line = 0;         //行号
		uint32_t m_elapse = 0;  //程序启动开始到现在的毫秒数
		uint32_t m_threadId = 0;     //线程 id
		uint32_t m_fiberId = 0;       //协程  id
		uint64_t m_time;             //时戳
		std::string m_content;        //消息

}


//日志级别
class LogLogLevel 
{
	public:
		enum Level 
		{
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			ERROR = 4,
			FATAL = 5
		};
};


//日志格式类
class LogFormatter
{
	typedef std::shared_ptr<LogFormatter> ptr;

	std::string format(LogEvent::ptr event);

};

//日志输出地
class LogAppender 
{
	public:
		typedef std::shared_ptr<LOgAppender> ptr;
		virtual ~LogAppender( ) {}

		void log( LogLevel::Level level,LogEvent::ptr event);
	private:
		LogLevel::Level m_level;


};


//日志器
class Logger
{
	public:
		typedef std::shared_ptr<Logger> ptr;

		Logger(const std::string& name = "root");

		void log( LogLevel::Level  level,LogEvent::ptr event);

		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent::ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);
		
		void addAppender ( LogAppender::ptr appender );
		void delAppender ( LogAppender::ptr appender );


		LogLevel::Level getLevel( ) const {return m_level ; }
		void setLevel( LogLevel::Level val) {m_level = val; }

	private:
		std::string m_name;    //日志名称
		LogLevel::Level m_level;  //满足日志级别的会输出
		std::list<LogAppender::ptr> m_appenders;  //Appeder 集合


};

//输出到控制台的 appender
class StdoutLogAppender : public LogAppender
{

};


//定义输出到文件的 appender 
class FileLogAppender : public LogAppender 
{

};


	
}



#endif
