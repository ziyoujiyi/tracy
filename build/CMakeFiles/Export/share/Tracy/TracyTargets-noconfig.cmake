#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Tracy::TracyClient" for configuration ""
set_property(TARGET Tracy::TracyClient APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Tracy::TracyClient PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libTracyClient.so.0.11.1"
  IMPORTED_SONAME_NOCONFIG "libTracyClient.so.0.11.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS Tracy::TracyClient )
list(APPEND _IMPORT_CHECK_FILES_FOR_Tracy::TracyClient "${_IMPORT_PREFIX}/lib/libTracyClient.so.0.11.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
