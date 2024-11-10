cmake_minimum_required(VERSION 3.23)
project(pixel2vga C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-gdwarf-4)

set(STB_IMAGE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image/include)

set(SOURCES
        src/vga.c src/vga.h
        src/image.c src/image.h)

add_executable(pixel2vga src/main.c ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${STB_IMAGE_INCLUDE_DIRS})
