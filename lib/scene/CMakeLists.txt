# lib/scene/CMakeLists.txt
add_library(scene solid.c)

if(UNIX)
  target_link_libraries(scene m vector)
endif(UNIX)
