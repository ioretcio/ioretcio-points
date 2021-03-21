cmake_minimum_required(VERSION 3.16)
project(Ariadne)
set(CMAKE_CXX_STANDARD 14)
SET(OpenCV_DIR /home/pi/installation/OpenCV-master/lib/cmake/opencv4)
find_package( OpenCV REQUIRED )
add_executable(Ariadne Ariadne.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "program.out")
target_link_libraries( Ariadne ${OpenCV_LIBS} )

