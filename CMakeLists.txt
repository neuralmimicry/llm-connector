cmake_minimum_required(VERSION 3.25)
project(llm_connector)

set(CMAKE_CXX_STANDARD 17)

add_executable(llm_connector src/llm_connector.cpp include/llm_connector.h)
