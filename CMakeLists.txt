cmake_minimum_required(VERSION 3.8)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(${RPI_RGB_LED_MATRIX_DIR}/include)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

add_executable(DisplayServer Main.cc LedMatrixDisplay.cc NetworkServer.cc)
target_link_libraries(DisplayServer rgbmatrix)
