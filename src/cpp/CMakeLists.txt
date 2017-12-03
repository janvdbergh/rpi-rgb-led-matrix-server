# Boost libraries
find_package(Boost 1.62 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

# Led matrix display libraries
set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(${RPI_RGB_LED_MATRIX_DIR}/include)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

# Led matrix display
add_executable(DisplayServerLed
        network/ServerMain.cpp
        network/NetworkError.cpp
        network/NetworkPacket.cpp
        network/NetworkServer.cpp
        display/Image.cpp
        display/Display.cpp
        display/LedMatrixDisplay.cpp
        network/NetworkError.h
        display/Image.cpp display/Image.h)
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES} pthread)

# Mock display
add_executable(DisplayServerMock
        network/ServerMain.cpp
        network/NetworkError.cpp
        network/NetworkPacket.cpp
        network/NetworkServer.cpp
        display/Image.cpp
        display/Display.cpp
        display/MockDisplay.cpp
        )
target_link_libraries(DisplayServerMock ${Boost_LIBRARIES} pthread)

# Client library
add_library(NetworkClient SHARED
        display/Image.cpp
        network/NetworkPacket.cpp
        network/NetworkError.cpp
        network/NetworkClient.cpp
        )
target_link_libraries(NetworkClient ${Boost_LIBRARIES})

# Demo
add_executable(Demo
        demo/ClientDemo.cpp
        )
target_link_libraries(Demo ${Boost_LIBRARIES} pthread NetworkClient)
