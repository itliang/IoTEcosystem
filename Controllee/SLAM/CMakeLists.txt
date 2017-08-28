cmake_minimum_required(VERSION 2.8)

project(SLAM C CXX)

# set build type
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set (CMAKE_BUILD_TYPE "Debug")
endif()

message (STATUS "build type: ${CMAKE_BUILD_TYPE}")


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set (CMAKE_C_FLAG "${CMAKE_C_FLAG} -Wall -fPIC -v -fvisibility=hidden")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -v -fvisibility=hidden -fvisibility-inlines-hidden -Wno-literal-suffix")
set (CMAKE_CXX_FLAGS_DEBUG "-O -g")

# configure external libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(E_GLIB_2_0 REQUIRED glib-2.0)
pkg_check_modules(E_EIGEN_3 REQUIRED eigen3)

#set (E_IOTIVITY 
#	c_common 
#	coap 
#	connectivity_abstraction 
#	logger 
#	oc_logger 
#	oc 
#	ocsrm 
#	octbstack 
#	resource_directory 
#	routingmanager
#)

set (EXTRA_LIBS
    E_GLIB_2_0
    E_EIGEN_3
)

foreach(flag ${EXTRA_LIBS})
    set (EXTRA_LIBS_INC       ${EXTRA_LIBS_INC}        ${${flag}_INCLUDE_DIRS})
    set (EXTRA_LIBS_DIRS 	  ${EXTRA_LIBS_DIRS}	   ${${flag}_LIBRARY_DIRS})
    set (EXTRA_LIBS_LINKER    ${EXTRA_LIBS_LINKER}     ${${flag}_LIBRARIES})
endforeach(flag)

message (STATUS "include external directories: ${EXTRA_LIBS_INC}\n")
message (STATUS "linking external libraries dirs: ${EXTRA_LIBS_DIRS}\n")
message (STATUS "linking external libraries: ${EXTRA_LIBS_LINKER}\n")

include_directories(
	${EXTRA_LIBS_INC}
	${CMAKE_SOURCE_DIR}/inc	
)

set(SRC
	${PROJECT_SOURCE_DIR}/src/slam.cpp	
)

# define MACROS
add_definitions(-DWITH_CLOUD)
# add_definitions(-DWITH_DEBUG)

# set RPATH
set (CMAKE_SKIP_BUILD_RPATH  FALSE)
set (CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/libs")
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} -l${EXTRA_LIBS_LINKER})
#target_link_libraries(${PROJECT_NAME} -L${PROJECT_SOURCE_DIR}/libs/iotivity -l${E_IOTIVITY})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -pthread LINK_FLAGS -pthread)

# set RPATH
set (CMAKE_SKIP_BUILD_RPATH  FALSE)
set (CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set (CMAKE_INSTALL_RPATH "${PROJECT_NAME}/libs")
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# install
#file (GLOB IOTIVITY_LIB_FILES ${PROJECT_SOURCE_DIR}/libs/iotivity/*)
#file (GLOB DATA_FILES ${PROJECT_SOURCE_DIR}/data/*)
#install (FILES ${IOTIVITY_LIB_FILES} DESTINATION ${PROJECT_NAME}/libs)
#install (FILES ${DATA_FILES} DESTINATION ${PROJECT_NAME}/data)
install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME}/bin)