get_filename_component(EXEC_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

#########################
# Dependencies download #
#########################

set(GREATEST_SHA256 "1292593d95c35eeccc89ffa1c91d6fe53b49f81cbf2c2b7758842b7f3186fcc2")
set(DOWNLOAD_DIR "${PROJECT_BINARY_DIR}/test_downloads")

file(MAKE_DIRECTORY "${DOWNLOAD_DIR}")
set(GREATEST_FILE "${DOWNLOAD_DIR}/greatest.h")
set(GREATEST_URL "https://raw.githubusercontent.com/silentbicycle/greatest/11a6af1/greatest.h")
file(DOWNLOAD "${GREATEST_URL}" "${GREATEST_FILE}"
        EXPECTED_HASH "SHA256=${GREATEST_SHA256}")

#############
# Configure #
#############

if (NOT DEFINED TARGET_ARCH)
    set(TARGET_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif (NOT DEFINED TARGET_ARCH)

configure_file("configure.h.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_configure.h" @ONLY)

#######################
# Test library target #
#######################

set(LIBRARY_NAME "${EXEC_NAME}_lib")

set(Header_Files "test_policy_to_json.h" "test_policy_from_json.h" "policy_mocks.h")
source_group("${LIBRARY_NAME} Header Files" FILES "${Header_Files}")

set(Source_Files "policy_mocks.c")
source_group("${LIBRARY_NAME} Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${LIBRARY_NAME}"
        PRIVATE
        "$<BUILD_INTERFACE:${DOWNLOAD_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if (MSVC)
    find_package(CURL CONFIG REQUIRED)
    set(CURL_LINK_LIBRARIES "CURL::libcurl")
else ()
    include(${CMAKE_SOURCE_DIR}/cmake/modules/FindCurlCustom.cmake)
    get_curl(CURL_LINK_LIBRARIES)
endif ()

#find_package(unofficial-parson CONFIG REQUIRED)
#find_package(libcurl-simple-https CONFIG REQUIRED)

target_link_libraries(
        "${LIBRARY_NAME}"
        PRIVATE
        #"${PROJECT_NAME}_compiler_flags"
        "${PROJECT_NAME}_pubsub"
)
if (NOT WIN32)
    target_compile_definitions("${LIBRARY_NAME}" PRIVATE C89STRINGUTILS_IMPLEMENTATION)
endif (NOT WIN32)

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

include(GenerateExportHeader)
set(_export_file "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

###############
# Test target #
###############

set(EXEC_NAME "${EXEC_NAME}_exec")

set(Source_Files "test.c")
source_group("${EXEC_NAME} Source Files" FILES "${Source_Files}")

add_executable("${EXEC_NAME}" "${Source_Files}")

target_include_directories(
        "${EXEC_NAME}"
        PRIVATE
        "$<BUILD_INTERFACE:${DOWNLOAD_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

#find_package(unofficial-parson CONFIG REQUIRED)
#find_package(libcurl-simple-https CONFIG REQUIRED)

target_link_libraries(
        "${EXEC_NAME}"
        PRIVATE
        #"${PROJECT_NAME}_compiler_flags"
        "${LIBRARY_NAME}"
        "${PROJECT_NAME}_pubsub"
)
if (NOT WIN32)
    target_compile_definitions("${EXEC_NAME}" PRIVATE C89STRINGUTILS_IMPLEMENTATION)
endif (NOT WIN32)

add_test(NAME "${EXEC_NAME}"
         COMMAND "${EXEC_NAME}"
         WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
