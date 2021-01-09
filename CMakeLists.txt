cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME argus)

project(${PROJECT_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

find_package(OpenCV 4.1.0 REQUIRED)
find_package(Boost 1.65 REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

set(PROJECT_SRCS
	main.cpp
	workers/image_library.cpp
	workers/image_library.h
)

# For using "workers/worker.h" in includes
include_directories(.)
# Boost directories
include_directories( ${Boost_INCLUDE_DIR} )

add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

target_link_libraries(${PROJECT_NAME}
	LINK_PRIVATE ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES
    apples.jpg
    flower.jpg
	parrot.jpg
DESTINATION bin)
