//##################################################################
// File Name: util.cpp
// File Effect: 
// Author: tttt
// mail: tttt@xiyoulinux.org
// Created Time: 2020年07月30日 星期四 20时05分11秒
//=============================================================
//!/usr/bin/python

#include "util.h"

namespace sylar
{
	pid_t GetThreadId()
	{

		return syscall(SYS_gettid);
	}

	uint32_t GetFiberId()
	{
		return 0;
	}
}
