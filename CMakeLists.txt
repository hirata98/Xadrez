cmake_minimum_required(VERSION 3.20)
# set the projectX name to the name of your project
project(xadrez)

set(CMAKE_CXX_STANDARD 20)
# set(SOURCE_FILES Servidor.cpp)
# set(SOURCE_FILES Cliente.cpp)

add_executable(Servidor Servidor.cpp Buttun.cpp Buttun.h chess.cpp chess.h win_lose.cpp win_lose.h)
add_executable(Cliente Cliente.cpp Buttun.cpp Buttun.h chess.cpp chess.h win_lose.cpp win_lose.h)
if(WIN32)
    target_link_libraries(Servidor wsock32 ws2_32)
    target_link_libraries(Cliente wsock32 ws2_32)
endif()
include_directories(C:\\SFML\\include)
link_directories(C:\\SFML\\build\\bin\\lib)

target_link_libraries(Servidor -lsfml-System -lsfml-Window -lsfml-Graphics -lsfml-Audio)
target_link_libraries(Cliente -lsfml-System -lsfml-Window -lsfml-Graphics -lsfml-Audio)