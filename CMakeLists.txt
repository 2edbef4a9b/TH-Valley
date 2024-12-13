cmake_minimum_required(VERSION 3.15)

# Set the project name.
project(THValley)

# Specific configurations for Xcode.
if(XCODE)
    set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
endif()

# Define an option for building tests.
option(BUILD_TESTS "Build tests" ON)

# Set the output directory if it isn't already set.
if(NOT CMAKE_BINARY_DIR)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
endif()

# Set the build type if it isn't already set.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set the output directories for the different build types.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/RelWithDebInfo)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/MinSizeRel)

# Set the runtime output directory based on the build type.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# Set cmake to export compile commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the compiler flags.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the compiler flags for the different toolchains.
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm2000 /Zi")
else()
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
        -fexceptions -Wno-deprecated-declarations -Wno-reorder"
    )
endif()

# Axmol engine configuration.
# Check if the Axmol engine is in the project directory.
set(IS_AXMOL_EMBED FALSE)

# Set the Axmol Engine root directory.
if(DEFINED ENV{AX_ROOT})
    # Use the system environment variable if it is set.
    file(TO_CMAKE_PATH "$ENV{AX_ROOT}" _AX_ROOT)
    message(STATUS
        "Using System Environment Variable AX_ROOT = "
        "${_AX_ROOT}"
    )
elseif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/axmol")
    # Use the embedded Axmol engine if the directory exists.
    file(GLOB_RECURSE AXMOL_ENGINE_CONTENTS
        "${CMAKE_CURRENT_SOURCE_DIR}/external/axmol/*"
    )

    if(AXMOL_ENGINE_CONTENTS)
        set(_AX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/axmol")
        set(IS_AXMOL_EMBED TRUE)
        message(STATUS
            "Building isolated project: ${PROJECT_NAME} with AX_ROOT = "
            "${_AX_ROOT}"
        )
    endif()
else()
    # Otherwise, throw an error.
    message(FATAL_ERROR
        "Environment variable AX_ROOT is not set and "
        "the Axmol engine is not embedded."
    )
endif()

# Initialize and set the cmake module path.
set(CMAKE_MODULE_PATH)
list(APPEND CMAKE_MODULE_PATH
    ${_AX_ROOT}/cmake/Modules
)

# Include the AXBuildSet module.
include(AXBuildSet)

# Include the subdirectories for the Axmol engine.
# We don't use prebuilt libraries for simplicity.
add_subdirectory(${_AX_ROOT}/core ${ENGINE_BINARY_PATH}/axmol/core)

# Project configuration.
# Set the resource folder for the application.
set(RESOURCES_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# Initialize and set the include directories.
set(INCLUDE_DIR)
list(APPEND INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/include/
    ${_AX_ROOT}/3rdparty/yaml-cpp/include/
)
include_directories(${INCLUDE_DIR})

# Set the source files.
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Function to link the resources.
function(link_resources TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/runtime/axslc
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/axslc
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        ${CMAKE_BINARY_DIR}/bin
        COMMENT "Creating symlink for /runtime/axslc and /assets in the "
        "CMAKE_RUNTIME_OUTPUT_DIRECTORY and removing /build/bin directory."
    )
endfunction()

# Set the application name.
set(APP_NAME th_valley)

# Add the application executable.
add_executable(${APP_NAME}
    ${SOURCE_FILES}
)

# Set the C++ standard for the application.
set_target_properties(${APP_NAME} PROPERTIES
    CXX_STANDARD 20
)

# Set the subsystem for the application on Windows.
if(WIN32)
    # Set the subsystem to Windows for a Console application.
    set_target_properties(${APP_NAME} PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# Link the application with the Axmol engine library.
target_link_libraries(${APP_NAME} ${_AX_CORE_LIB})

# Copy the dynamic-link libraries on Windows.
if(WINDOWS)
    ax_sync_target_dlls(${APP_NAME})
endif()

# Set up the axmol application configuration.
ax_setup_app_config(${APP_NAME})
ax_setup_app_props(${APP_NAME})

# Link the resources.
link_resources(${APP_NAME})

# Unit testing configuration.
# Set the source files without main.cpp for unit testing.
set(SOURCE_FILES_WITHOUT_MAIN ${SOURCE_FILES})
list(REMOVE_ITEM SOURCE_FILES_WITHOUT_MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Conditionally find and fetch GoogleTest if BUILD_TESTS is ON.
if(BUILD_TESTS)
    # Find the GoogleTest library.
    find_package(GTest, QUIET)

    # Use the installed GTest if it is found, otherwise download and build it.
    if(GTest_FOUND)
        message(STATUS "Found installed GTest, using local library.")
    else()
        message(STATUS
            "GTest not found, using FetchContent to download and build."
        )

        include(FetchContent)

        # Cache the downloaded files to avoid downloading them again.
        set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/.cmakecache")

        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/heads/main.zip
        )
        FetchContent_MakeAvailable(googletest)
    endif()
endif()

# Enable CTest.
enable_testing()

# Function to add a test.
function(add_googletest TEST_MODULE TEST_NAME)
    # Add the test executable.
    add_executable(${TEST_NAME}
        ${SOURCE_FILES_WITHOUT_MAIN}
        tests/${TEST_MODULE}/${TEST_NAME}.cpp
    )

    # Set the C++ standard for the test.
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 20
    )

    # Link the test with the GoogleTest and GoogleMock libraries.
    target_link_libraries(${TEST_NAME}
        gtest
        gtest_main
        gmock
        ${_AX_CORE_LIB}
    )

    # Add the test to CTest.
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )

    # Set up the axmol application configuration.
    ax_setup_app_config(${TEST_NAME})
    ax_setup_app_props(${TEST_NAME})

    # Link the resources.
    link_resources(${TEST_NAME})
endfunction()

# Conditionally add the tests if BUILD_TESTS is ON.
if(BUILD_TESTS)
    add_googletest(sample sample_application_tests)
endif()
