get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")

include(GenerateExportHeader)

set(Header_Files
        "compute.h"
        "compute/cloud_auth.h"
        "compute/compute_common.h"
        "compute/firewall.h"
        "compute/instance.h"
        "compute/network.h"
        "compute/zones.h"
)

source_group("Header Files" FILES "${Header_Files}")

set(Source_Files
        "compute.c"
        "compute/cloud_auth.c"
        "compute/compute_common.c"
        "compute/firewall.c"
        "compute/instance.c"
        "compute/network.c"
        "compute/zones.c"
        )
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
        #"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/compute>"
        #"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../morestd>"
        "$<INSTALL_INTERFACE:include>"
)

find_package(unofficial-parson CONFIG REQUIRED)
set(_libs "unofficial::parson::parson"
        "libcurl_simple_https"
        "common"
        "oauth2")

set(_pub_libs "${PROJECT_LOWER_NAME}_compiler_flags")
if (NOT _GNU_SOURCE AND NOT HAVE_ASPRINTF)
  list(APPEND _pub_libs "morestd")
  # target_link_libraries("${LIBRARY_NAME}" PUBLIC "morestd")
endif (NOT _GNU_SOURCE AND NOT HAVE_ASPRINTF)

target_link_libraries("${LIBRARY_NAME}" PRIVATE "${_libs}")

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${_pub_libs}")

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")
target_compile_definitions(
        "${LIBRARY_NAME}"
        PUBLIC
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>:"${LIBRARY_NAME}"_STATIC_DEFINE=1>
)

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "1.0.0")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "1")

# install rules
set(installable_libs "${LIBRARY_NAME}" "${PROJECT_LOWER_NAME}_compiler_flags")
if (TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
install(TARGETS ${installable_libs}
        DESTINATION "${INSTALL_DEST}"
        EXPORT "${LIBRARY_NAME}Targets")
install(FILES "${_export_file}" "${Header_Files}" DESTINATION "include")
