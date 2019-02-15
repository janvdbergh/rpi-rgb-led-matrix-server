# Boost libraries
find_package(Boost 1.62 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIR})

# Led matrix display_old libraries
set(RPI_RGB_LED_MATRIX_DIR $ENV{HOME}/libs/rpi-rgb-led-matrix)
include_directories(
		${RPI_RGB_LED_MATRIX_DIR}/include
		./include
)
link_directories(${RPI_RGB_LED_MATRIX_DIR}/lib)

# Display server LED display
add_executable(DisplayServerLed
		impl/common/Command.cpp

		impl/server/ServerMain.cpp
		impl/common/NetworkPacket.cpp
		impl/server/NetworkServer.cpp

		impl/server/Layer.cpp
		impl/server/LayerMap.cpp
		impl/common/Image.cpp
		impl/server/ImageMap.cpp
		impl/server/DisplayError.cpp
		impl/server/LedMatrixDisplay.cpp

		impl/server/ServerCommand.cpp

		impl/server/ExecutionContext.cpp
)
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES} pthread)

# Installation of server
install(
		TARGETS DisplayServerLed
		DESTINATION rpi-rgb-led-matrix-server/bin
)
