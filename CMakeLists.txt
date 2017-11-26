cmake_minimum_required(VERSION 3.8)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

# Boost libraries
find_package(Boost 1.65 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

# Led matrix display libraries
set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(${RPI_RGB_LED_MATRIX_DIR}/include)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

# Network files
set(NETWORK_FILES
        src/NetworkError.cpp
        src/NetworkPacket.cpp
        src/NetworkServer.cpp
        )

# Led matrix display
add_executable(DisplayServerLed
        src/ServerMain.cpp
        src/NetworkError.cpp
        src/NetworkPacket.cpp
        src/NetworkServer.cpp
        src/Display.cpp
        src/LedMatrixDisplay.cpp
        src/NetworkError.h
        )
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES})

# Mock display
add_executable(DisplayServerMock
        src/ServerMain.cpp
        src/NetworkError.cpp
        src/NetworkPacket.cpp
        src/NetworkServer.cpp
        src/Display.cpp
        src/MockDisplay.cpp
        )
target_link_libraries(DisplayServerMock ${Boost_LIBRARIES})

add_executable(demo
        demo/ClientDemo.cpp
        src/NetworkPacket.cpp
        src/NetworkError.cpp
        src/NetworkClient.cpp
        src/NetworkClient.h
        )
target_link_libraries(demo ${Boost_LIBRARIES})
