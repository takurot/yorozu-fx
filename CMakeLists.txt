cmake_minimum_required(VERSION 3.31)
project(yorozu_fx VERSION 0.1.0 LANGUAGES CXX)

if(APPLE)
    if(NOT CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
        message(FATAL_ERROR "Configure macOS with -DCMAKE_OSX_ARCHITECTURES=arm64")
    endif()
    set(OFX_PLATFORM_DIR MacOS)
elseif(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|amd64|x86_64")
    set(OFX_PLATFORM_DIR Win64)
else()
    message(FATAL_ERROR "P01 supports native macOS arm64 and Windows x64 builds")
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build configuration" FORCE)
endif()

add_library(openfx_headers INTERFACE)
target_include_directories(openfx_headers SYSTEM INTERFACE third_party/openfx/include)

add_library(yorozu_ofx MODULE src/ofx/plugin.cpp)
target_link_libraries(yorozu_ofx PRIVATE openfx_headers)
target_compile_features(yorozu_ofx PRIVATE cxx_std_17)
set(BUNDLE_ROOT "${CMAKE_BINARY_DIR}/bundle/$<CONFIG>/yorozu-fx.ofx.bundle")
set_target_properties(yorozu_ofx PROPERTIES
    PREFIX "" SUFFIX ".ofx" OUTPUT_NAME "yorozu-fx"
    CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN YES
    LIBRARY_OUTPUT_DIRECTORY "${BUNDLE_ROOT}/Contents/${OFX_PLATFORM_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${BUNDLE_ROOT}/Contents/${OFX_PLATFORM_DIR}")
add_custom_command(TARGET yorozu_ofx POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BUNDLE_ROOT}/Contents/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/openfx/LICENSE.md"
        "${BUNDLE_ROOT}/Contents/Resources/openfx-LICENSE.md")
if(APPLE)
    configure_file(cmake/Info.plist.in Info.plist @ONLY)
    add_custom_command(TARGET yorozu_ofx POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" "${BUNDLE_ROOT}/Contents/Info.plist")
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(ofx_abi_smoke tests/ofx_abi_smoke.cpp)
    target_compile_features(ofx_abi_smoke PRIVATE cxx_std_17)
    target_link_libraries(ofx_abi_smoke PRIVATE openfx_headers ${CMAKE_DL_LIBS})
    add_dependencies(ofx_abi_smoke yorozu_ofx)
    add_test(NAME ofx_abi_smoke COMMAND ofx_abi_smoke "$<TARGET_FILE:yorozu_ofx>")
    add_test(NAME ofx_missing_module COMMAND ofx_abi_smoke "${CMAKE_BINARY_DIR}/absent.ofx")
    set_tests_properties(ofx_missing_module PROPERTIES WILL_FAIL TRUE)
    add_test(NAME ofx_package COMMAND ${CMAKE_COMMAND}
        "-DBUNDLE_ROOT=${BUNDLE_ROOT}" "-DPLATFORM_DIR=${OFX_PLATFORM_DIR}"
        "-DSOURCE_ROOT=${CMAKE_CURRENT_SOURCE_DIR}" -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/check_package.cmake")
    add_test(NAME openfx_integrity COMMAND ${CMAKE_COMMAND}
        "-DSDK_ROOT=${CMAKE_CURRENT_SOURCE_DIR}/third_party/openfx"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/check_sdk.cmake")
endif()

foreach(target yorozu_ofx ofx_abi_smoke)
    if(TARGET ${target})
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4 /WX)
        else()
            target_compile_options(${target} PRIVATE -Wall -Wextra -Werror)
        endif()
    endif()
endforeach()
