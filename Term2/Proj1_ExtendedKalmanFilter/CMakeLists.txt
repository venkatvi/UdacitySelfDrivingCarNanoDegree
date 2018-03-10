project(ExtendedKF)

cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++14)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

#set(sources src/main.cpp src/tools.cpp src/FusionEKF.cpp src/kalman_filter.cpp src/tools.h src/FusionEKF.h #src/kalman_filter.h)

set(sources src/main.cpp src/DataParser.h src/DataAdapter.h src/DataAdapter.cpp src/ExtendedKalmanFilter.cpp src/ExtendedKalmanFilter.h src/KalmanFilter.cpp src/KalmanFilter.h src/LaserMeasurement.cpp src/LaserMeasurement.h src/LaserStateAdapterStrategy.h src/Measurement.h src/RadarMeasurement.cpp src/RadarMeasurement.h src/RadarStateAdapterStrategy.h src/SensorFusionApplication.cpp src/SensorFusionApplication.h src/State.cpp src/State.h src/StateAdapterStrategy.h src/StateAdapterStrategyFactory.h src/Utils.cpp src/Utils.h) 


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

include_directories(/usr/local/include)
include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/opt/openssl/lib)
link_directories(/usr/local/Cellar/libuv/1*/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 


add_executable(ExtendedKF ${sources})

target_link_libraries(ExtendedKF z ssl uv uWS)