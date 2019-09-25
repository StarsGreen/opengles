cmake_minimum_required(VERSION 3.2)

PROJECT(camera)

aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

if(PLATFORM STREQUAL "x86" OR PLATFORM STREQUAL "")
   message(STATUS "x86 platform")
   set(BIN_NAME openglES-x86)
   file(WRITE ${PROJECT_SOURCE_DIR}/include/config.h 
   "
   /* 0x01 stand for x86 arch while 0x00 for arm */\r
    #define PLATFORM 0x01
    ")
elseif (PLATFORM STREQUAL "arm")
   message(STATUS "arm platform")
   set(BIN_NAME openglES-arm)
   file(WRITE ${PROJECT_SOURCE_DIR}/include/config.h 
   "
   /* 0x01 stand for x86 arch while 0x00 for arm */\r
    #define PLATFORM 0x00
   ")
else()
   message(STATUS "platform error")
endif(PLATFORM STREQUAL "x86" OR PLATFORM STREQUAL "")

#message(STATUS "the pro dir" ${PROJECT_SOURCE_DIR})

add_executable(${BIN_NAME} ${DIR_SRCS})

message(STATUS "the src dir" ${DIR_SRCS})


if(PLATFORM STREQUAL "x86" OR PLATFORM STREQUAL "")
   message(STATUS "including x86 head files and linking x86 library")
   include_directories(${PROJECT_SOURCE_DIR}/include /usr/include)
   target_link_libraries(${BIN_NAME} EGL)
   target_link_libraries(${BIN_NAME} GLESv2)
   target_link_libraries(${BIN_NAME} GLESv1_CM)
   target_link_libraries(${BIN_NAME} X11)
   target_link_libraries(${BIN_NAME} opencv_highgui)
   target_link_libraries(${BIN_NAME} opencv_core)
   target_link_libraries(${BIN_NAME} opencv_imgcodecs)
   target_link_libraries(${BIN_NAME} opencv_imgproc)

elseif (PLATFORM STREQUAL "arm")
   message(STATUS "including arm head files and linking arm library")
   set(CMKAE_SYSTEM_NAME Linux)
   set(CMKAE_SYSTEM_PROCESSOR arm)
   set(TOOLCHAIN_PATH /usr/local/gcc-arm-linux-gnueabihf)
   #set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
   #set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
   #set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
   
   set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-gcc)
   set(CMAKE_C_FLAGS "-I /usr/include -L ${PROJECT_SOURCE_DIR}/lib")
   set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-g++)
   set(CMAKE_CXX_FLAGS "-I /usr/include  -L ${PROJECT_SOURCE_DIR}/lib") 

   set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH} ${TOOLCHAIN_PATH}/include ${TOOLCHAIN_PATH}/lib)
   #target_include_directories(${BIN_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
   #include_directories(/usr/include)
   #include_directories(/usr/local/include)

   target_include_directories(${BIN_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include /usr/include /usr/local/include)
   target_link_libraries(${BIN_NAME} EGL)
   target_link_libraries(${BIN_NAME} GLESv2)
   target_link_libraries(${BIN_NAME} GLESv1_CM)
  # target_link_libraries(${BIN_NAME} opencv_highgui)
  # target_link_libraries(${BIN_NAME} opencv_core)
  # target_link_libraries(${BIN_NAME} opencv_imgcodecs)
  # target_link_libraries(${BIN_NAME} opencv_imgproc)
  # find_library(LIBEGL_DIR EGL ${PROJECT_SOURCE_DIR}/lib)
  # find_library(LIBGLESv2_DIR GLESv2 ${PROJECT_SOURCE_DIR}/lib)
  # find_library(LIBGLESv1_CM_DIR GLESv1_CM ${PROJECT_SOURCE_DIR}/lib)
else()
   message(STATUS "platform error")
endif(PLATFORM STREQUAL "x86" OR PLATFORM STREQUAL "")
