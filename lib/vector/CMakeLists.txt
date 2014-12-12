# lib/vector/CMakeLists.txt
add_library(vector vector.c)

if(UNIX)
  target_link_libraries(vector m)
endif(UNIX)
