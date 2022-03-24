get_filename_component(DIRNAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(LIBRARY_NAME "${PROJECT_NAME}_${DIRNAME}")

set(Header_Files
        "cloud_auth.h"
        "compute_common.h"
        "firewall.h"
        "instance.h"
        "network.h"
        "zones.h"
        )

source_group("Header Files" FILES "${Header_Files}")

set(Source_Files
        "cloud_auth.c"
        "compute_common.c"
        "firewall.c"
        "instance.c"
        "network.c"
        "zones.c"
        )
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

include(GNUInstallDirs)
get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
get_filename_component(PROJECT_ROOT_DIR "${PROJECT_ROOT_DIR}" DIRECTORY)
target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>"
        "$<BUILD_INTERFACE:${PROJECT_ROOT_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

find_package(unofficial-parson CONFIG REQUIRED)
find_package(libcurl-simple-https CONFIG REQUIRED)

if (MSVC)
    find_package(CURL CONFIG REQUIRED)
    set(CURL_LINK_LIBRARIES CURL::libcurl)
else ()
    include(${CMAKE_SOURCE_DIR}/cmake/modules/FindCurlCustom.cmake)
    get_curl(CURL_LINK_LIBRARIES)
endif ()

set(_libs "unofficial::parson::parson"
        "libcurl-simple-https"
        "${CURL_LINK_LIBRARIES}")

target_link_libraries("${LIBRARY_NAME}" PRIVATE "${_libs}")

find_package(cauthflow CONFIG REQUIRED)
target_link_libraries(
        "${LIBRARY_NAME}"
        PUBLIC
        "cauthflow"
        "cauthflow_utils"
        "cauthflow_google_auth"
        "cauthflow_compiler_flags"
        "${PROJECT_NAME}_multi"
)

find_path(C89STRINGUTILS_INCLUDE_DIRS "c89stringutils_string_extras.h")

target_include_directories("${LIBRARY_NAME}" PRIVATE "${C89STRINGUTILS_INCLUDE_DIRS}")

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${PROJECT_NAME}_common")

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

include(GenerateExportHeader)
set(_export_file "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${DIRNAME}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "1.0.0")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "1")

# install rules
install(FILES "${_export_file}" ${Header_Files}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${DIRNAME}")
