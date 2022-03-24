add_library("${PROJECT_NAME}" INTERFACE)
foreach (lib "common" "compute" "project" "storage" "userinfo" "multi")
  add_subdirectory("${lib}")
  set(lib_name "${PROJECT_NAME}_${lib}")
  list(APPEND installable_libs "${lib_name}")
  target_link_libraries("${PROJECT_NAME}" INTERFACE "${lib_name}")
endforeach (lib "common" "compute" "project" "storage" "userinfo" "multi")

# install rules
include(GNUInstallDirs)
list(APPEND installable_libs "${PROJECT_NAME}_compiler_flags")
if (TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif (TARGET "${DEPENDANT_LIBRARY}")
install(TARGETS ${installable_libs}
        EXPORT "${PROJECT_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(EXPORT "${PROJECT_NAME}Targets" DESTINATION "${CMAKE_INSTALL_DATADIR}/${LIBRARY_NAME}")
