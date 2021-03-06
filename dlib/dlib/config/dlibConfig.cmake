# ===================================================================================
#  The dlib CMake configuration file
#
#             ** File generated automatically, do not modify **
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#
#    FIND_PACKAGE(dlib REQUIRED)
#    TARGET_LINK_LIBRARIES(MY_TARGET_NAME ${dlib_LIBRARIES})
#
#    This file will define the following variables:
#      - dlib_LIBRARIES                : The list of all imported targets for dlib modules.
#      - dlib_INCLUDE_DIRS             : The dlib include directories.
#      - dlib_VERSION                  : The version of this dlib build.
#      - dlib_VERSION_MAJOR            : Major version part of this dlib revision.
#      - dlib_VERSION_MINOR            : Minor version part of this dlib revision.
#
# ===================================================================================


# Compute paths
get_filename_component(dlib_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_LESS 2.8)
    get_filename_component(dlib_INSTALL_PATH "${dlib_CMAKE_DIR}/../../../" ABSOLUTE)
else()
    get_filename_component(dlib_INSTALL_PATH "${dlib_CMAKE_DIR}/../../../" REALPATH)
endif()

set(dlib_INCLUDE_DIRS "${dlib_INSTALL_PATH}/include")
 
# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET dlib-shared AND NOT dlib_BINARY_DIR)
    include("${dlib_CMAKE_DIR}/dlib.cmake")
endif()

find_library(dlib_LIBRARIES dlib HINTS ${dlib_INSTALL_PATH}/lib)
set(dlib_LIBRARIES ${dlib_LIBRARIES} "-lpthread;/usr/lib/arm-linux-gnueabihf/libnsl.so;/usr/lib/arm-linux-gnueabihf/libSM.so;/usr/lib/arm-linux-gnueabihf/libICE.so;/usr/lib/arm-linux-gnueabihf/libX11.so;/usr/lib/arm-linux-gnueabihf/libXext.so;/usr/lib/arm-linux-gnueabihf/libpng.so;/usr/lib/arm-linux-gnueabihf/libjpeg.so")
