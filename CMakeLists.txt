cmake_minimum_required(VERSION 3.8)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.65 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

# Led matrix display
set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(${RPI_RGB_LED_MATRIX_DIR}/include)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

# Network files
set(NETWORK_FILES
        network/Errors.cpp
        network/Packet.cpp
        network/Server.cpp
        )

# Led matrix display

add_executable(DisplayServerLed
        Main.cpp
        ${NETWORK_FILES}
        displays/Display.cpp
        displays/LedMatrixDisplay.cpp
        network/Errors.h
        )
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES})

# Mock display
add_executable(DisplayServerMock
        Main.cpp
        ${NETWORK_FILES}
        displays/Display.cpp
        displays/MockDisplay.cpp
        )
target_link_libraries(DisplayServerMock ${Boost_LIBRARIES})

add_executable(Test test/Test.cpp ${NETWORK_FILES})
target_link_libraries(Test ${Boost_LIBRARIES})
