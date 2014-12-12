# lib/material/CMakeLists.txt
add_library(material material.c)

if(UNIX)
  target_link_libraries(material m)
endif(UNIX)
