CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(hackIllinois)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0020 NEW)
endif(COMMAND cmake_policy)
ADD_DEFINITIONS(-std=c++11)

#Qt
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5Core)
#Qt

find_package( OpenCV REQUIRED )


ADD_EXECUTABLE(run main.cpp mainwindow.cpp face.cpp)
TARGET_LINK_LIBRARIES(run Qt5::Widgets)
TARGET_LINK_LIBRARIES(run Qt5::Core)
TARGET_LINK_LIBRARIES(run ${OpenCV_LIBS} )
INCLUDE_DIRECTORIES("../eigen")
