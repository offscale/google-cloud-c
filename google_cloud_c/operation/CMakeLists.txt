get_filename_component(DIRNAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(LIBRARY_NAME "${PROJECT_NAME}_${DIRNAME}")

set(Header_Files "operation.h")
source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "operation.c")
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
    set(CURL_LINK_LIBRARIES "CURL::libcurl")
else ()
    include(${CMAKE_SOURCE_DIR}/cmake/modules/FindCurlCustom.cmake)
    get_curl(CURL_LINK_LIBRARIES)
endif ()

target_link_libraries(
        "${LIBRARY_NAME}"
        PUBLIC
        "unofficial::parson::parson"
        "libcurl-simple-https"
        "${CURL_LINK_LIBRARIES}"
)

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
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "${${PROJECT_NAME}_VERSION}")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}")

# install rules
install(FILES "${_export_file}" ${Header_Files}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${DIRNAME}")
