#生成CMakeList.txt
cmake_minimum_required(VERSION 3.10)
project(hello)

#将src目录下的所有源文件放入变量SRC_LIST中
aux_source_directory(src SRC_LIST)

#指定头文件
include_directories(include)

#生成可执行文件
add_executable(hello ${SRC_LIST})
