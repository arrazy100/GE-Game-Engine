#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tmx" for configuration ""
set_property(TARGET tmx APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(tmx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libtmx.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tmx )
list(APPEND _IMPORT_CHECK_FILES_FOR_tmx "${_IMPORT_PREFIX}/lib/libtmx.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
