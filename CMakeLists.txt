cmake_minimum_required (VERSION 2.8)
project (rt)

include(CheckCXXCompilerFlag)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

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

add_executable(${PROJECT_NAME} ${GLOBAL_SOURCES} ${SOURCES} )

# STB
set(STB_LIB_DIR "${LIB_DIR}/stb")
target_include_directories(${PROJECT_NAME} PRIVATE ${STB_LIB_DIR})


add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND rm -r CMakeCache.txt cmake_install.cmake CMakeFiles Makefile
)

add_custom_target(run
   COMMAND make
   COMMAND ./rt
)