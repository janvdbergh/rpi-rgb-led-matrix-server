cmake_minimum_required(VERSION 3.7)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src/cpp)

install(
		FILES fonts/*
		DESTINATION rpi-rgb-led-matrix-server/bin/fonts
)