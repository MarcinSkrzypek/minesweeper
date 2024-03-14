cmake_minimum_required(VERSION 3.10)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES 
    "*.cpp"
    "src/*.cpp"
)
file(GLOB HEADERS "include/*.h")
set(RESOURCES resources/diffDialog.rc)

add_executable(minesweeper WIN32 ${SOURCES} ${HEADERS} ${RESOURCES})
target_link_libraries(minesweeper gdi32)
target_link_options(minesweeper PRIVATE -mwindows)
add_compile_options(-Wall)
