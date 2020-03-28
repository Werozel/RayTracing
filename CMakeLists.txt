cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fopenmp -foffload='-lm'")

project (rt)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set (CMAKE_CXX_STANDARD 11)

add_executable(rt main.cpp vectors.cpp objects.cpp)

add_custom_target(ALL 
   COMMAND export OMP_NUM_THREADS=32
   COMMAND g++ -fopenmp -foffload="-lm" main.cpp vectors.cpp objects.cpp -o rt -O3
)

add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND rm -r CMakeCache.txt cmake_install.cmake CMakeFiles Makefile
)

add_custom_target(run
   COMMAND make
   COMMAND ./rt
)

add_custom_target(omp
   COMMAND export OMP_NUM_THREADS=32
   COMMAND g++ -fopenmp -foffload="-lm" main.cpp vectors.cpp objects.cpp -o rt -O3
)


target_link_libraries(rt ${ALL_LIBS} )

