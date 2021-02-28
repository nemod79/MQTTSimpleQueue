cmake_minimum_required(VERSION 3.5)

project(SampleMQTT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(SampleMQTT src/SampleMQTT.cpp src/MosquittoPublisher.cpp src/MosquittoSubscriber.cpp)
TARGET_LINK_LIBRARIES(SampleMQTT pthread mosquitto)
