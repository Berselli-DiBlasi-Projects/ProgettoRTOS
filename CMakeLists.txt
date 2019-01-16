cmake_minimum_required(VERSION 3.1)
project(image_processing)
include_directories(.)
set(CMAKE_GCC_FLAGS "-Wall -Wextra -Wconversion -pedantic -std=gnu11")

set(THREADS_PREFER_PTHREAD_FLAG ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM gtkmm-3.0)
PKG_CHECK_MODULES(X11 x11)
find_package(OpenCV REQUIRED)
find_package(Allegro REQUIRED)
find_package(Threads REQUIRED)
find_package(ptask REQUIRED)
find_package(SDL2 REQUIRED)

set(ptask_INCLUDE_DIRS "${ptask_DIR}/include")
message("Include dir ${ptask_INCLUDE_DIRS}")

add_executable(application src/main.cpp src/image_processing.cpp src/camera.cpp
	       src/filter.cpp src/frame_difference.cpp src/FrmMain.cpp
	       src/FrmSettings.cpp src/histogram.cpp src/output_manager.cpp
               src/thread_manager.cpp src/threshold.cpp)

include_directories ("${ptask_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS} )
include_directories(${ALLEGRO_INCLUDE_DIR})

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

include_directories(${X11_INCLUDE_DIR})

target_link_libraries(application ${X11_LIBRARIES} ${OpenCV_LIBS} ${GTKMM_LIBRARIES} ptask Threads::Threads ${ALLEGRO_LIBRARY} rt m)
