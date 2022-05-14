get_filename_component(EXEC_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
set(EXEC_NAME "${EXEC_NAME}_exec")

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

###############
# Test target #
###############

set(Header_Files "test_policy_to_json.h")
source_group("${EXEC_NAME} Header Files" FILES "${Header_Files}")

set(Source_Files "test.c")
source_group("${EXEC_NAME} Source Files" FILES "${Source_Files}")

add_executable("${EXEC_NAME}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${EXEC_NAME}"
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
        "${EXEC_NAME}"
        PRIVATE
        #"${PROJECT_NAME}_compiler_flags"
        "${PROJECT_NAME}_pubsub"
)
target_compile_definitions("${EXEC_NAME}" PRIVATE C89STRINGUTILS_IMPLEMENTATION)

add_test(NAME "${EXEC_NAME}"
         COMMAND "${EXEC_NAME}"
         WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")
