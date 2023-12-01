# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\qschematic-demo_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\qschematic-demo_autogen.dir\\ParseCache.txt"
  "qschematic-demo_autogen"
  )
endif()
