cmake_minimum_required(VERSION 3.25)
project(llm_connector)

set(CMAKE_CXX_STANDARD 17)

# Look for required packages
find_package(CURL REQUIRED)

add_executable(llm_connector src/llm_connector.cpp)

# Specify include directories for the target
target_include_directories(llm_connector PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

# Link libraries to the target
target_link_libraries(llm_connector PRIVATE
        ${CURL_LIBRARIES}
        pthread
        )

# Specify compile options
target_compile_options(llm_connector PRIVATE -Wall -Wextra)
