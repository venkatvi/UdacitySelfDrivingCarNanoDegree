project(ExtendedKF)

cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++14)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

#set(sources src/main.cpp src/tools.cpp src/FusionEKF.cpp src/kalman_filter.cpp src/tools.h src/FusionEKF.h #src/kalman_filter.h)

set(sources src/main.cpp 
src/DataUtils/InputParser.h src/DataUtils/InputParser.cpp src/DataUtils/DataTypes.h 
src/DataUtils/DataReaderFactory.h src/DataUtils/DataReaderFactory.cpp
src/DataUtils/DataReader.h src/DataUtils/DataReader.cpp 
src/DataUtils/WebClientReader.h src/DataUtils/WebClientReader.cpp src/DataUtils/FileReader.h src/DataUtils/FileReader.cpp src/DataUtils/DataAdapter.h src/DataUtils/DataAdapter.cpp src/ExtendedKalmanFilter.cpp src/ExtendedKalmanFilter.h src/KalmanFilter.cpp src/KalmanFilter.h 
src/SensorFusionApplication.cpp src/SensorFusionApplication.h 
src/Data/Measurement.h src/Data/LaserMeasurement.h  src/Data/RadarMeasurement.h 
src/Data/LaserStateAdapterStrategy.h 
src/Data/LaserStateAdapterStrategy.cpp 
src/Data/RadarStateAdapterStrategy.h 
src/Data/RadarStateAdapterStrategy.cpp 
src/Data/State.cpp src/Data/State.h 
src/Data/StateAdapterStrategy.h 
src/Data/StateAdapterStrategyFactory.h src/Data/StateAdapterStrategyFactory.cpp 
src/DataUtils/Utils.cpp src/DataUtils/Utils.h) 


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

include_directories(/usr/local/include)
include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/opt/openssl/lib)
link_directories(/usr/local/Cellar/libuv/1*/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 


add_executable(ExtendedKF ${sources})

target_link_libraries(ExtendedKF z ssl uv uWS)
