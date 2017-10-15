cmake_minimum_required(VERSION 3.8)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 11)

add_executable(DisplayServer Main.cc LedMatrixDisplay.cc NetworkServer.cc)
