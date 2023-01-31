cmake_minimum_required(VERSION 3.8)

project(SDL-GUI VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH 
"F:\\Tools\\SDL2\\SDL2-2.26.1"
"F:\\Tools\\SDL2\\SDL2_ttf-2.20.1"
"F:\\Tools\\SDL2\\SDL2_image-2.6.2")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_compile_definitions(CMAKE_BUILD)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

aux_source_directory(SCore S_Core)
aux_source_directory(SGUI S_Gui)
include_directories(./SCore ./SGUI)

add_executable(SDL-demo "main.cpp" 
${S_Core}
${S_Gui})

target_link_libraries(SDL-demo SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
