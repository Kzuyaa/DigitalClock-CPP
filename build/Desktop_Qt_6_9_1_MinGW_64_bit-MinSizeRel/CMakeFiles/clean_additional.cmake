# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles\\DigitalClock_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DigitalClock_autogen.dir\\ParseCache.txt"
  "DigitalClock_autogen"
  )
endif()
