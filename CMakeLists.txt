# 学习 cmake
# 时建议直接从命令列表作为入口，遇到不清楚意思的命令都可以在此处查阅：
# https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html
# 另外下面是完整的文档入口：
# https://cmake.org/cmake/help/latest/

# 设置运行该脚本所需的 cmake 最低版本
cmake_minimum_required(VERSION 3.9)

# 使用 set 命令设置一个变量
set(PROJECT_NAME localfuse)


# 设置项目名称
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 指定所有源码文件
# 使用了 cmake 的 file 命令，递归查找项目目录下所有头文件和源码文件，
# 并将结果放入 SRCS 变量中，SRCS 变量可用于后续使用

file(GLOB CORE_SRCS
    "src/*.c"
    "src/*.h"
)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64 " )

set(LINK_LIBRARIES
    fuse
)

add_compile_definitions(CVERSION="${CVERSION}")

add_executable(${PROJECT_NAME}
    ${CORE_SRCS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LINK_LIBRARIES}
)


# 设置安装路径的前缀(默认为"/usr/local")
set(CMAKE_INSTALL_PREFIX "/usr")


# 设置执行 make install 时哪个目标应该被 install 到哪个位置
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

