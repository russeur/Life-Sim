cmake_minimum_required(VERSION 4.1.0)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
project(Galaga VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_compile_options(-Wall)




add_executable(main
src/main.cpp
src/drawer.cpp
src/world.cpp
src/high_res_clock.cpp
)

target_include_directories(main PRIVATE 
headers
${SDL2_INCLUDE_DIRS}
${SDL2_TTF_INCLUDE_DIRS})




target_link_libraries(main PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_ttf::SDL2_ttf)
