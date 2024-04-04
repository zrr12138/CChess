cmake_minimum_required(VERSION 3.16)
project(CChess)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(common)
add_subdirectory(third-party)

# 添加源文件
set(SOURCES ChessBoard.cpp
        MCTSEngine.cpp
        )

# 添加头文件路径
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/third-party/Crow/include
        ${PROJECT_SOURCE_DIR}/third-party/glog/src
        ${CMAKE_BINARY_DIR}/third-party/gflags/include
)

# 设置构建类型为 Debug
set(CMAKE_BUILD_TYPE Debug)

# 设置调试相关的编译选项
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
# 生成可执行文件

add_library(CChess_lib ${SOURCES})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux环境下链接tcmalloc
    target_link_libraries(CChess_lib tcmalloc glog::glog gflags common_lib)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin") # macOS
    # macOS环境下添加/usr/local/include到头文件搜索路径
    include_directories(/usr/local/include)
    target_link_libraries(CChess_lib glog::glog gflags common_lib)
else ()
    message(FATAL_ERROR "Project CChess does not support Windows platform, because some reason about pthread")
endif ()

add_executable(server server.cpp)
target_link_libraries(server CChess_lib)

add_executable(ChessBoradTest ChessBoardTest.cpp)
target_link_libraries(ChessBoradTest CChess_lib)

add_executable(EngineTest EngineTest.cpp)
target_link_libraries(EngineTest CChess_lib)