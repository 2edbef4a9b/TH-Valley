cmake_minimum_required(VERSION 3.15)

# Set the project name.
project(THValley)

# Define an option for building tests.
set(BUILD_TESTS TRUE)

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

# Set cmake to export compile commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the compiler flags.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the compiler flags for the different toolchains.
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /EHsc")
else()
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
        -no-pie -fexceptions -Wno-deprecated-declarations -Wno-reorder"
    )
endif()

# Set the COCOS2DX_ROOT_PATH with the environment variable.
if(DEFINED ENV{COCOS_X_ROOT})
    set(COCOS2DX_ROOT_PATH $ENV{COCOS_X_ROOT})
else()
    message(FATAL_ERROR "Environment variable COCOS_X_ROOT is not set.")
endif()

# Initialize and set the cmake module path.
set(CMAKE_MODULE_PATH)
list(APPEND CMAKE_MODULE_PATH
    ${COCOS2DX_ROOT_PATH}/cmake/Modules
)

# Set the resource folder for the application.
set(RESOURCES_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# Initialize and set the include directories.
set(INCLUDE_DIR)
list(APPEND INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/include/
    ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)
include_directories(${INCLUDE_DIR})

# Set the source files.
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Set the source files without main.cpp for unit testing.
set(SOURCE_FILES_WITHOUT_MAIN ${SOURCE_FILES})
list(REMOVE_ITEM SOURCE_FILES_WITHOUT_MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Include the cocos framework.
include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# Conditionally find and fetch GoogleTest if BUILD_TESTS is TRUE.
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

# Link resources to the target.
function(link_resources TARGET_NAME)
    set(APP_RESOURCE_DIR "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets")

    if(WIN32)
        # Copy the resources on Windows.
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCES_DIR} ${APP_RESOURCE_DIR}
        )
    else()
        # Create a symbolic link to the resources on Unix.
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${RESOURCES_DIR} ${APP_RESOURCE_DIR}
        )
    endif()
endfunction()

# Set the application name.
set(APP_NAME th_valley)

# Add the application executable.
add_executable(${APP_NAME}
    ${SOURCE_FILES})

# Set the C++ standard for the application.
set_target_properties(${APP_NAME} PROPERTIES
    CXX_STANDARD 20
)

# Link the application with the cocos2d library.
target_link_libraries(${APP_NAME} cocos2d)

# Copy resources to the output directory.
link_resources(${APP_NAME})

# Copy the dynamic-link libraries on Windows.
if(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

# Enable CTest.
enable_testing()

# Function to add a test.
function(add_googletest TEST_MODULE TEST_NAME)
    # Add the test executable.
    add_executable(${TEST_NAME}
        ${SOURCE_FILES_WITHOUT_MAIN}
        tests/${TEST_MODULE}/${TEST_NAME}.cpp)

    # Set the C++ standard for the test.
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 20
    )

    # Link the test with the GoogleTest and GoogleMock libraries.
    target_link_libraries(${TEST_NAME}
        gtest
        gtest_main
        gmock
        cocos2d
    )

    # Add the test to CTest.
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )

    # Copy resources to the output directory.
    link_resources(${TEST_NAME})
endfunction()

# Conditionally add the tests if BUILD_TESTS is ON.
if(BUILD_TESTS)
    add_googletest(sample sample_application_tests)
    add_googletest(frontend title_screen_tests)
    add_googletest(tool_bar tool_bar_tests)
endif()
