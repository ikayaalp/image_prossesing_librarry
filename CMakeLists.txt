cmake_minimum_required(VERSION 3.16)
project(ImageProcessingLib VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-specific settings
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-DNOMINMAX)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/Image.cpp
    src/GaussianBlur.cpp
    src/EdgeDetection.cpp
    src/BrightnessAdjust.cpp
    src/FilterPipeline.cpp
    src/ImageViewer.cpp
)

# Header files
set(HEADERS
    include/Image.h
    include/Filter.h
    include/GaussianBlur.h
    include/EdgeDetection.h
    include/BrightnessAdjust.h
    include/FilterPipeline.h
    include/ImageViewer.h
)

# Create library
add_library(imageprocessing ${SOURCES} ${HEADERS})
target_link_libraries(imageprocessing sfml-graphics sfml-window sfml-system)

# Set library properties
set_target_properties(imageprocessing PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${HEADERS}"
)

# Create executables
add_executable(image_demo src/main.cpp)
target_link_libraries(image_demo imageprocessing)

# Kenar bulma programı
add_executable(kenar_bulma src/kenar_bulma.cpp)
target_link_libraries(kenar_bulma imageprocessing)

# Windows-specific executable settings
if(WIN32)
    set_target_properties(image_demo PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Add vesikalik_filtre if source exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/vesikalik_filtre.cpp)
    add_executable(vesikalik_filtre src/vesikalik_filtre.cpp)
    target_link_libraries(vesikalik_filtre imageprocessing)
endif()

# Testing
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS imageprocessing DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)

# Package configuration
set(CPACK_PACKAGE_NAME "ImageProcessingLib")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION "Modern C++ Image Processing Library")
set(CPACK_PACKAGE_VENDOR "ImageProcessing Team")
set(CPACK_PACKAGE_CONTACT "your-email@example.com")

# Platform-specific package settings
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "ImageProcessingLib")
    set(CPACK_NSIS_PACKAGE_NAME "ImageProcessingLib")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ImageProcessing Team")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev")
endif()

include(CPack)