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

# Display server client library
add_library(DisplayClient
		impl/common/Command.cpp
		impl/common/Image.cpp
		impl/common/NetworkPacket.cpp
		impl/common/NetworkPacketParser.cpp
		impl/common/NetworkPacketBuilder.cpp
)
set_target_properties(DisplayClient PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)
install(
		FILES
			include/common/Command.h
			include/common/Image.h
			include/common/NetworkError.h
			include/common/NetworkPacket.h
			include/common/NetworkPacketBuilder.h
			include/common/Pixel.h
			include/common/Point.h
		DESTINATION rpi-rgb-led-matrix-server/include
)
install(
		TARGETS DisplayClient
		DESTINATION rpi-rgb-led-matrix-server/lib
)

# Display server LED display
add_executable(DisplayServerLed
		impl/common/Command.cpp
		impl/common/Image.cpp
		impl/common/NetworkPacket.cpp
		impl/common/NetworkPacketBuilder.cpp
		impl/common/NetworkPacketParser.cpp

		impl/server/DisplayError.cpp
		impl/server/ExecutionContext.cpp
		impl/server/ImageMap.cpp
		impl/server/Layer.cpp
		impl/server/LayerMap.cpp
		impl/server/LedMatrixDisplay.cpp
		impl/server/NetworkServer.cpp
		impl/server/ServerCommand.cpp
		impl/server/ServerMain.cpp
)
set_target_properties(DisplayServerLed PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(DisplayServerLed rgbmatrix ${Boost_LIBRARIES} pthread)
install(
		TARGETS DisplayServerLed
		DESTINATION rpi-rgb-led-matrix-server/bin
)
