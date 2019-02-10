# Boost libraries
find_package(Boost 1.62 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

# Led matrix display_old libraries
set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(${RPI_RGB_LED_MATRIX_DIR}/include)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

# Display server LED display
add_executable(DisplayServerLed
		command/Command.cpp

#		network/ServerMain.cpp
#		network/NetworkError.cpp
		network/NetworkPacket.cpp
#		network/NetworkServer.cpp

		display/Layer.cpp
		display/LayerMap.cpp
		datatype/Image.cpp
		display/ImageMap.cpp
		display/DisplayError.cpp
#		display/LedMatrixDisplay.cpp

		interpreter/ServerCommand.cpp

		interpreter/ExecutionContext.cpp
)
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES} pthread)

# Installation of server
install(
		TARGETS DisplayServerLed
		DESTINATION rpi-rgb-led-matrix-server/bin
)
