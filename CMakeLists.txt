# CMakeLists.txt
cmake_minimum_required (VERSION 2.6)
project (raytracer)

set (RAYTRACER_LIB_DIR ${RAYTRACER_LIB_DIR} ${RAYTRACER_SOURCE_DIR}/lib)

set(CMAKE_BUILD_TYPE Profile)
set(CMAKE_CXX_FLAGS_PROFILE "-Wall -g -pg")
set(CMAKE_C_FLAGS_PROFILE "-Wall -g -pg")

# cloc line count report
# add_custom_command(
#   OUTPUT ${RAYTRACER_SOURCE_DIR}/cloc.txt
#   COMMAND perl cloc.pl ./ --by-file-by-lang --exclude-dir=CMakeFiles,doc --exclude-lang=C++,CMake,make,Perl --out=cloc.txt
#   DEPENDS raytracer
# )
# add_custom_target(cloc_report ALL DEPENDS ${RAYTRACER_SOURCE_DIR}/cloc.txt)

# # profiling, gprof
# add_custom_command(
#   OUTPUT ${RAYTRACER_SOURCE_DIR}/profile.txt
#   COMMAND gprof raytracer gmon.out > profile.txt
#   DEPENDS raytracer
# )
# add_custom_target(profile DEPENDS ${RAYTRACER_SOURCE_DIR}/profile.txt)

include_directories(
  "./lib/vector"
  "./lib/ray"
  "./lib/scene"
  "./lib/material"
  "./lib/image"
)

add_subdirectory("./lib/vector")
add_subdirectory("./lib/ray")
add_subdirectory("./lib/scene")
add_subdirectory("./lib/material")
add_subdirectory("./lib/image")

link_directories(${RAYTRACER_LIB_DIR})

add_executable(raytracer main.c)

if(UNIX)
  target_link_libraries(raytracer m)
endif(UNIX)

target_link_libraries(raytracer vector ray scene material image)
