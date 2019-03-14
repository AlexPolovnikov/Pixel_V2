cmake_minimum_required (VERSION 3.8)
project (PixelSum)

set(SOURCE_EXE PixelSum.cpp)
set(SOURCE_LIB Tests.cpp Source.cpp)
set(HEADER pch.h Tests.h Header.h)
add_library(Tests STATIC ${SOURCE_LIB})
add_executable(main ${SOURCE_EXE})
target_link_libraries(main Tests)	