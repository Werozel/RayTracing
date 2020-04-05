cmake_minimum_required (VERSION 2.8)
project (rt)

include(CheckCXXCompilerFlag)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(OBJ_DIR "${LIB_DIR}/obj")
set(BACKGROUND_DIR "${LIB_DIR}/backgrounds")
set(STB_LIB_DIR "${LIB_DIR}/stb")


function(enable_cxx_compiler_flag_if_supported flag)
string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
if(flag_already_set EQUAL -1)
check_cxx_compiler_flag("${flag}" flag_supported)
if(flag_supported)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
endif()
unset(flag_supported CACHE)
endif()
endfunction()


enable_cxx_compiler_flag_if_supported("-std=c++17")
enable_cxx_compiler_flag_if_supported("-fopenmp")
enable_cxx_compiler_flag_if_supported("-O3")

file(GLOB GLOBAL_SOURCES *.h *.cpp)
file(GLOB SOURCES src/*.cpp stb/*.cpp)
file(COPY "${OBJ_DIR}/*.obj" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/obj")
message(STATUS "Copied OBJ files")
file(COPY "${BACKGROUND_DIR}/*.jpg" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/background")
message(STATUS "Copied JPG files")

add_executable(${PROJECT_NAME} ${GLOBAL_SOURCES} ${SOURCES} )

target_include_directories(${PROJECT_NAME} PRIVATE ${OBJ_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${BACKGROUND_DIR})


# STB
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_LIB_DIR})


add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND rm -r CMakeCache.txt cmake_install.cmake CMakeFiles Makefile
)

add_custom_target(run
   COMMAND make
   COMMAND ./rt
)

add_custom_target(debug
   COMMAND g++ -fopenmp main.cpp src/*cpp -o rt -g
)