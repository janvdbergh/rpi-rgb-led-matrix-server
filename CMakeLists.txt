cmake_minimum_required(VERSION 3.8)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.65 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Led matrix display
set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(${RPI_RGB_LED_MATRIX_DIR}/include)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

add_executable(DisplayServerLed
        Main.cpp
        server/NetworkServer.cpp
        displays/Display.cpp
        displays/LedMatrixDisplay.cpp
        )
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES})

add_executable(DisplayServerMock
        Main.cpp
        server/NetworkServer.cpp
        displays/Display.cpp
        displays/MockDisplay.cpp
        )
target_link_libraries(DisplayServerMock ${Boost_LIBRARIES})
