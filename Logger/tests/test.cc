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

int main()
{
	sylar::Logger::ptr logger(new sylar::Logger);

	logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

	sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__,__LINE__,0,1,2,time(0)));

	logger->log(sylar::LogLevel::DEBUG,event);



	return 0;
}
