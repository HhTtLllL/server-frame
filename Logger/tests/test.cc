//##################################################################
// File Name: test.cpp
// File Effect: 
// Author: tttt
// mail: tttt@xiyoulinux.org
// Created Time: 2020年07月30日 星期四 13时35分49秒
//=============================================================
//!/usr/bin/python

#include <iostream>
#include "../sylar/log.h"
#include "../sylar/util.h"

int main()
{
	sylar::Logger::ptr logger(new sylar::Logger);
	logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));


	sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));
	
	sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));

	file_appender->setFormatter(fmt);
	file_appender->setLevel(sylar::LogLevel::ERROR);

	logger->addAppender(file_appender);


	//sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__,__LINE__,0,sylar::GetThreadId(),sylar::GetFiberId(),time(0)));

	//logger->log(sylar::LogLevel::DEBUG,event);
	
	SYLAR_LOG_INFO(logger) << "test macro";

	SYLAR_LOG_FMT_ERROR(logger,"test macro fmt error %s","aa");

	auto l = sylar::LoggerMgr::GetInstance()->getLogger("XX");
	SYLAR_LOG_INFO(l) << "XXX";


	return 0;
}
