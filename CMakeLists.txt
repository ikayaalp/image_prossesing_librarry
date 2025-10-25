cmake_minimum_required(VERSION 3.16)
project(GorselIsleme VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(include)

set(SOURCES
    src/Image.cpp
    src/GaussianBlur.cpp
    src/EdgeDetection.cpp
    src/BrightnessAdjust.cpp
    src/FilterPipeline.cpp
    src/ImageViewer.cpp
)

set(HEADERS
    include/Image.h
    include/Filter.h
    include/GaussianBlur.h
    include/EdgeDetection.h
    include/BrightnessAdjust.h
    include/FilterPipeline.h
    include/ImageViewer.h
)

add_library(gorselisleme ${SOURCES} ${HEADERS})
target_link_libraries(gorselisleme sfml-graphics sfml-window sfml-system)

add_executable(image_demo src/main.cpp)
target_link_libraries(image_demo gorselisleme)

add_executable(kenar_bulma src/kenar_bulma.cpp)
target_link_libraries(kenar_bulma gorselisleme)