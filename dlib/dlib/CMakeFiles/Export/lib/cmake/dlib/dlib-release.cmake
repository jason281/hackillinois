#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dlib::dlib" for configuration "Release"
set_property(TARGET dlib::dlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(dlib::dlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "-lpthread;/usr/lib/arm-linux-gnueabihf/libnsl.so;/usr/lib/arm-linux-gnueabihf/libSM.so;/usr/lib/arm-linux-gnueabihf/libICE.so;/usr/lib/arm-linux-gnueabihf/libX11.so;/usr/lib/arm-linux-gnueabihf/libXext.so;/usr/lib/arm-linux-gnueabihf/libpng.so;/usr/lib/arm-linux-gnueabihf/libjpeg.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libdlib.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS dlib::dlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_dlib::dlib "${_IMPORT_PREFIX}/lib/libdlib.a" )

# Import target "dlib::dlib_shared" for configuration "Release"
set_property(TARGET dlib::dlib_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(dlib::dlib_shared PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "-lpthread;/usr/lib/arm-linux-gnueabihf/libnsl.so;/usr/lib/arm-linux-gnueabihf/libSM.so;/usr/lib/arm-linux-gnueabihf/libICE.so;/usr/lib/arm-linux-gnueabihf/libX11.so;/usr/lib/arm-linux-gnueabihf/libXext.so;/usr/lib/arm-linux-gnueabihf/libpng.so;/usr/lib/arm-linux-gnueabihf/libjpeg.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libdlib.so.19.0.0"
  IMPORTED_SONAME_RELEASE "libdlib.so.19.0.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS dlib::dlib_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_dlib::dlib_shared "${_IMPORT_PREFIX}/lib/libdlib.so.19.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)