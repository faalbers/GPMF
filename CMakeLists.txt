cmake_minimum_required(VERSION 3.4)
project(GPMF VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#add_compile_definitions(MP4_PARSE_PATH)
#add_compile_definitions(MP4_PARSE_DATA)
add_compile_definitions(MP4_PARSE_TIME)
#add_compile_definitions(GPMF_PARSE_PATH)
#add_compile_definitions(GPMF_PARSE_DATA)
add_compile_definitions(GPMF_PARSE_TIME)

add_subdirectory(extern/MP4)

file(GLOB KLVS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} klvs/*.cpp )
#file(GLOB DATA RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} data/*.cpp )
#add_library(GPMF GPMF.cpp ${KLVS} ${DATA})
add_library(GPMF GPMF.cpp ${KLVS})
target_include_directories(GPMF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GPMF PUBLIC MP4)

add_executable(testGPMF testGPMF.cpp)
target_link_libraries(testGPMF PRIVATE GPMF )
