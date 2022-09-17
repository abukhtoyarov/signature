cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(APP_NAME signature)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(${APP_NAME} CXX)

# bootstrap conan
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

# signature target
add_executable(${APP_NAME}
    src/main.cc
    src/arg_parser.cc
    src/md5.cc
    src/processing.cc
    src/reader.cc
    src/worker.cc
    src/writer.cc
    src/file.cc
    )

target_link_libraries(${APP_NAME}
    # TODO: For test case purposes it does not matter but future should be changed
    # to separated libraries, e.g. Boost::program_options
    CONAN_PKG::boost
    CONAN_PKG::cryptopp
    )

target_compile_options(${APP_NAME} PUBLIC
    $<$<COMPILE_LANG_AND_ID:CXX,gcc,clang>:-Wall>
    $<$<COMPILE_LANG_AND_ID:CXX,gcc,clang>:-Werror>
    )

# unit test target
add_executable(runUnitTests
    test/arg_parser.cc
    src/arg_parser.cc
    test/md5.cc
    src/md5.cc
    )

target_compile_options(runUnitTests PUBLIC
    $<$<COMPILE_LANG_AND_ID:CXX,gcc,clang>:-Wall>
    $<$<COMPILE_LANG_AND_ID:CXX,gcc,clang>:-Werror>
    )

target_link_libraries(runUnitTests
    CONAN_PKG::gtest
    # TODO: For test case purposes it does not matter but future should be changed
    # to separated libraries, e.g. Boost::program_options
    CONAN_PKG::boost
    CONAN_PKG::cryptopp
    )

include(GoogleTest)
enable_testing()
gtest_add_tests(TARGET runUnitTests TEST_LIST allTests)
gtest_discover_tests(runUnitTests)

# copy compile_commands.json target
add_custom_target(
    copy-compile-commands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_LIST_DIR}
    )

