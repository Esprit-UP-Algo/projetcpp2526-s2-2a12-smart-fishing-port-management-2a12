# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles\\appStockageFrigorifique_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appStockageFrigorifique_autogen.dir\\ParseCache.txt"
  "appStockageFrigorifique_autogen"
  )
endif()
