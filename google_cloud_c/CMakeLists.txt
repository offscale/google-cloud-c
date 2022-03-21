get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")

set(Header_Files
        "compute.h"
        "compute/cloud_auth.h"
        "compute/compute_common.h"
        "compute/firewall.h"
        "compute/instance.h"
        "compute/network.h"
        "compute/zones.h"

        "project/project.h"

        "storage/notification_configs.h"

        "userinfo/userinfo.h"
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

        "project/project.c"

        "storage/notification_configs.c"

        "userinfo/userinfo.c"
        )
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

include(GNUInstallDirs)
target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
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

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${_pub_libs}")

find_package(cauthflow CONFIG REQUIRED)
target_link_libraries(
        "${LIBRARY_NAME}"
        PRIVATE
        cauthflow cauthflow_utils
        cauthflow_google_auth cauthflow_compiler_flags)

find_path(C89STRINGUTILS_INCLUDE_DIRS "c89stringutils_string_extras.h")

target_include_directories("${LIBRARY_NAME}" PRIVATE "${C89STRINGUTILS_INCLUDE_DIRS}")

target_link_libraries("${LIBRARY_NAME}" PRIVATE "${PROJECT_NAME}_common")

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

include(GenerateExportHeader)
set(_export_file "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "1.0.0")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "1")

# install rules
set(installable_libs "${PROJECT_NAME}_compiler_flags" "${PROJECT_NAME}_common" "${LIBRARY_NAME}")
if (TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif (TARGET "${DEPENDANT_LIBRARY}")
install(TARGETS ${installable_libs}
        EXPORT "${LIBRARY_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES "${_export_file}"
        TYPE "INCLUDE")
foreach(Header_File ${Header_Files})
  get_filename_component(base_name "${Header_File}" NAME)
  if (base_name STREQUAL "${Header_File}")
    install(FILES "${Header_File}" TYPE "INCLUDE")
  else ()
    get_filename_component(base_dir "${Header_File}" DIRECTORY)
    install(FILES "${Header_File}"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${base_dir}")
  endif ()
endforeach(Header_File ${Header_Files})

install(EXPORT "${LIBRARY_NAME}Targets" DESTINATION "${CMAKE_INSTALL_DATADIR}/${LIBRARY_NAME}")
