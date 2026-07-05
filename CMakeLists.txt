cmake_minimum_required(VERSION 3.10)

project(calculator VERSION 2.0.0)

include(FetchContent)

FetchContent_Declare(
	libmath
	GIT_REPOSITORY git@github.com:redbirb5/libmath.git
	GIT_TAG homework-2
)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(
    libmath 
    json
    googletest
    spdlog
)

find_package(PostgreSQL REQUIRED)

add_library(calculator_app
    src/app.cpp
    src/calculator.cpp
    src/logger.cpp
    src/calculation_service.cpp
    src/calculation_cache.cpp
    src/calculation_storage.cpp
    src/postgres_connection.cpp
)

target_link_libraries(calculator_app
	PRIVATE
	    math
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        PostgreSQL::PostgreSQL
)

target_include_directories(calculator_app
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(calculator
	src/main.cpp
)

target_link_libraries(calculator
	PRIVATE
        calculator_app
)

enable_testing()

add_executable(calculator_tests
    tests/app_test.cpp
)

target_link_libraries(calculator_tests
    PRIVATE
        calculator_app
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(calculator_tests)

install(TARGETS calculator
	RUNTIME DESTINATION /usr/local/bin
)

option(ENABLE_CLANG_FORMAT "Enable clang-format" OFF)
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_PATH NAMES clang-format-19 clang-format REQUIRED)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
        src/*.cpp
	    src/*.h
    )

    add_custom_target(calculator-clang-format-target
        COMMAND
            ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
        WORKING_DIRECTORY
            ${CMAKE_SOURCE_DIR}
        COMMENT
            "Formatting with ${CLANG_FORMAT_PATH} ..."
    )

    add_dependencies(calculator_app calculator-clang-format-target)
endif()

option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_PATH NAMES clang-tidy-19 clang-tidy REQUIRED)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_PATH}")

    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_PATH} "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    set_target_properties(calculator_app calculator PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()
