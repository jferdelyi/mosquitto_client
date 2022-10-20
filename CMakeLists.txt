cmake_minimum_required(VERSION 3.22)
project(mosquitto_client)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(MOSQUITTOPP REQUIRED)
list(APPEND SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mqtt.cpp
)
include_directories(${MOSQUITTOPP_INCLUDE_DIR})
link_directories(${MOSQUITTOPP_LIBRARY}/..)

add_executable(mosquitto_client ${SOURCES})
target_link_libraries(mosquitto_client mosquittopp)
