cmake_minimum_required(VERSION 3.21)
project(ImageProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(ImageProject main.cpp
        Image.cpp
        Image.h
        Button.cpp
        Button.h
)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/cmake/SFML)
find_package(SFML 3 COMPONENTS System Window Graphics REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SFML/include)
target_link_libraries(ImageProject PRIVATE SFML::System SFML::Window SFML::Graphics)

