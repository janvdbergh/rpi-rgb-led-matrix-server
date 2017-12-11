cmake_minimum_required(VERSION 3.7)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src/cpp)

install(
		FILES fonts/6x9.bdf fonts/9x18.bdf
		DESTINATION rpi-rgb-led-matrix-server/bin/fonts
)