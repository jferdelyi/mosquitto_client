# - Find libmosquittopp
# Find the native libmosquittopp includes and libraries
#
#  MOSQUITTOPP_INCLUDE_DIR - where to find mosquitto.h, etc.
#  MOSQUITTOPP_LIBRARIES   - List of libraries when using libmosquittopp.
#  MOSQUITTOPP_FOUND       - True if libmosquittopp found.

if (WIN32)
    set(MOSQUITTOPP_INCLUDE_DIR "$ENV{Path}" )
    set(MOSQUITTOPP_LIBRARY "$ENV{Path}" )
    link_directories("$ENV{Path}")
endif (WIN32)

if (NOT MOSQUITTOPP_INCLUDE_DIR)
    find_path(MOSQUITTOPP_INCLUDE_DIR mosquitto.h)
endif()

if (NOT MOSQUITTOPP_LIBRARY)
    find_library(MOSQUITTOPP_LIBRARY NAMES mosquittopp)
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
        MOSQUITTOPP DEFAULT_MSG
        MOSQUITTOPP_LIBRARY MOSQUITTOPP_INCLUDE_DIR)

message(STATUS "libmosquittopp include dir: ${MOSQUITTOPP_INCLUDE_DIR}")
message(STATUS "libmosquittopp: ${MOSQUITTOPP_LIBRARY}")
set(MOSQUITTOPP_LIBRARIES ${MOSQUITTOPP_LIBRARY})

mark_as_advanced(MOSQUITTOPP_INCLUDE_DIR MOSQUITTOPP_LIBRARY)