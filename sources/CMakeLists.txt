cmake_minimum_required (VERSION 2.8)

cmake_policy (SET CMP0003 OLD)
cmake_policy (SET CMP0002 OLD)
cmake_policy (SET CMP0011 NEW)
cmake_policy (SET CMP0015 NEW)

project (reportabug)

set (SUBPROJECT reportabug)
set (PROJECT_AUTHOR "Evgeniy Alekseev")
set (PROJECT_CONTACT "esalexeev@gmail.com")
set (PROJECT_LICENSE "GPLv3")
set (PROJECT_VERSION_MAJOR 1)
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)
set (PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
string (TIMESTAMP CURRENT_DATE "%Y-%m-%d %H:%M" UTC)
string (TIMESTAMP CURRENT_YEAR "%Y")

message (STATUS "Subproject: ${SUBPROJECT}")
message (STATUS "Version: ${PROJECT_VERSION}")
message (STATUS "Build date: ${CURRENT_DATE}")

# install options
option (USE_QT5 "Use Qt5 instead of Qt4" ON)

# flags
if (CMAKE_COMPILER_IS_GNUCXX)
    set (ADD_CXX_FLAGS  "-Wall")
    set (CMAKE_CXX_FLAGS "-O0 ${ADD_CXX_FLAGS}")
    set (CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
else ()
    message (STATUS "Unknown compiler")
endif ()

configure_file (${CMAKE_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

# build
# set directories
set (SUBPROJECT_BINARY_DIR bin)
set (SUBPROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# executable path
set (EXECUTABLE_OUTPUT_PATH ${SUBPROJECT_BINARY_DIR})

# additional targets
set (TARGETS "")
set (HEADERS "")

add_subdirectory (${SUBPROJECT_SOURCE_DIR})

install (FILES ${SUBPROJECT}.desktop DESTINATION share/applications/)
