cmake_minimum_required(VERSION 3.7)
project(DisplayServer)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src/cpp)

install(
		FILES fonts/6x9.bdf fonts/9x18.bdf
		DESTINATION rpi-rgb-led-matrix-server/bin/fonts
)
install(
		FILES scripts/rpi-rgb-led-matrix-server
		DESTINATION /etc/init.d
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
)