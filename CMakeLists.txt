cmake_minimum_required(VERSION 3.1)
# (REFERENCE CODE) project(video_capture)
project(ball)
include_directories(.)

set(CMAKE_GCC_FLAGS "-Wall -Wextra -Wconversion  -pedantic -std=gnu11")

set(THREADS_PREFER_PTHREAD_FLAG ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM gtkmm-3.0)
find_package(OpenCV REQUIRED)
find_package(Allegro REQUIRED)
find_package(Threads REQUIRED)
find_package(ptask REQUIRED)
find_package(SDL2 REQUIRED)

set(ptask_INCLUDE_DIRS "${ptask_DIR}/include")
message("Include dir ${ptask_INCLUDE_DIRS}")

# (REFERENCE CODE) add_executable(video_capture task.c video_capture.c)
add_executable(application src/main.cpp src/FrmMain.cpp)

include_directories ("${ptask_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS} )
include_directories(${ALLEGRO_INCLUDE_DIR})
# include_directories(${SDL2_INCLUDE_DIRS})

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Add other flags to the compiler
# ADD_DEFINITIONS(${GTKMM_CFLAGS_OTHER})

# (REFERENCE CODE) target_link_libraries(video_capture ${OpenCV_LIBS} ptask Threads::Threads ${ALLEGRO_LIBRARY} rt m)
target_link_libraries(application ${OpenCV_LIBS} ${GTKMM_LIBRARIES} ptask Threads::Threads ${ALLEGRO_LIBRARY} rt m)
