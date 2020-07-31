cmake_minimum_required(VERSION 2.8)   # 定义最小版本
project(sylar)  #定义一个项目名

set(CMAKE_VERBOSE_MAKEFILE ON)

#自定义一些编译参数放进去
set( CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

#把源码 放到 LIB_SRC目录下
set(LIB_SRC
	sylar/log.cpp
	sylar/util.cpp
	)


#增加了一个 SHARED 库， 用于生成so 文件
add_library(sylar SHARED ${LIB_SRC})
#add_library(sylar_static STATIC ${LIB_SRC})
#SET_TARGET_PORPERTIES (sylar_static PROPERTIES OUTPUT_NAME "sylar")

#测试的文件
add_executable(test tests/test.cc)

add_dependencies(test sylar)
target_link_libraries(test sylar)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

