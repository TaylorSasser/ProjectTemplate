cmake_minimum_required(VERSION 3.10)
project(project_template VERSION 1.0.0 LANGUAGES CXX C ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


add_executable(project_template
        src/main.cpp
    src/project.cpp
)

add_executable(
    project_template::project_template ALIAS project_template
)

target_include_directories(
    project_template
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


include (cmake/CMakeRC.cmake)
cmrc_add_resource_library(
    resources
        assets/.clang-format.template
        assets/.clang-tidy.template
        assets/.gitignore.template
        assets/.gitattributes.template
        assets/include/project_template/factorial.template
        assets/src/factorial.template
        assets/executable/CMakeLists.template
        assets/executable/src/main.template
        assets/library/CMakeLists.template
        assets/library/example/CMakeLists.template
        assets/library/example/main.template
        assets/library/tests/CMakeLists.template
        assets/library/tests/example_test.template
)
set_property(TARGET resources PROPERTY POSITION_INDEPENDENT_CODE ON)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(project_template PUBLIC fmt::fmt)
target_link_libraries(project_template PUBLIC resources)

target_compile_features(project_template PRIVATE
    cxx_auto_type
    cxx_lambdas
    cxx_trailing_return_types
    cxx_variadic_macros
)

target_compile_options(project_template PRIVATE)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/project_template_utils)

install(
    TARGETS project_template
    EXPORT project_template_targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/project_template_config_version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES DESTINATION ${INSTALL_CONFIGDIR})

export(
    EXPORT project_template_targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/project_template_targets.cmake
    NAMESPACE project_template::
)

export(PACKAGE project_template)
