# 设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 3.0) 
project(mprpc)

# cmake默认生成版本是release，如果需要对程序进行调试，则需要设置下面的参数
set(CMAKE_BUILD_TYPE "Debug") 

# 设置项目可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)

#设置项目库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
# 设置项目链接目录
link_libraries(protobuf)

# src包含了mrpc框架所有的相关代码
add_subdirectory(src)
# example包含了mrpc框架使用的示例代码
add_subdirectory(example)