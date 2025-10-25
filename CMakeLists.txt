cmake_minimum_required(VERSION 3.16)
project(GorselIsleme VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML bul
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include dizini
include_directories(include)

# Kaynak dosyalar
set(SOURCES
    src/Image.cpp
    src/GaussianBlur.cpp
    src/EdgeDetection.cpp
    src/BrightnessAdjust.cpp
    src/FilterPipeline.cpp
    src/ImageViewer.cpp
)

# Header dosyalar
set(HEADERS
    include/Image.h
    include/Filter.h
    include/GaussianBlur.h
    include/EdgeDetection.h
    include/BrightnessAdjust.h
    include/FilterPipeline.h
    include/ImageViewer.h
)

# Ana kutuphane
add_library(gorselisleme ${SOURCES} ${HEADERS})
target_link_libraries(gorselisleme sfml-graphics sfml-window sfml-system)

# Executable'lar
add_executable(image_demo src/main.cpp)
target_link_libraries(image_demo gorselisleme)

add_executable(kenar_bulma src/kenar_bulma.cpp)
target_link_libraries(kenar_bulma gorselisleme)

# Test desteği
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()
    
    # Test executable
    add_executable(gorselisleme_test
        tests/test_image.cpp
        tests/test_filters.cpp
        tests/test_pipeline.cpp
        tests/test_integration.cpp
    )
    
    target_link_libraries(gorselisleme_test 
        gorselisleme 
        GTest::GTest 
        GTest::Main
        sfml-graphics 
        sfml-window 
        sfml-system
    )
    
    # Test'leri ekle
    add_test(NAME gorselisleme_tests COMMAND gorselisleme_test)
endif()

# CPack konfigurasyonu
set(CPACK_PACKAGE_NAME "gorselisleme")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Gorsel Isleme Kutuphanesi")
set(CPACK_PACKAGE_VENDOR "Ogrenci Projesi")

include(CPack)