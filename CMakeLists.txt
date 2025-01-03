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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
    add_definitions(-D_DISABLE_VECTOR_ANNOTATION -D_DISABLE_STRING_ANNOTATION)
else()
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} \
        -no-pie -fexceptions -Wno-deprecated-declarations -Wno-reorder"
    )
endif()

# Initialize the cmake module path and include directories.
set(CMAKE_MODULE_PATH)
set(INCLUDE_DIR)

# Cocos2d-x configuration.
# Set the COCOS2DX_ROOT_PATH with the environment variable.
if(DEFINED ENV{COCOS_X_ROOT})
    cmake_path(CONVERT $ENV{COCOS_X_ROOT} TO_CMAKE_PATH_LIST COCOS2DX_ROOT_PATH)
else()
    message(FATAL_ERROR "Environment variable COCOS_X_ROOT is not set.")
endif()

# Append cocos2d-x cmake modules to the module path.
list(APPEND CMAKE_MODULE_PATH
    ${COCOS2DX_ROOT_PATH}/cmake/Modules
)

# Append cocos2d-x audio engine include directory to the include directories.
list(APPEND INCLUDE_DIR
    ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# Include the cocos2d-x framework.
include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# Boost configuration.
# Find the Boost library.
if(WINDOWS)
    if(DEFINED ENV{BOOST_ROOT} AND
        DEFINED ENV{BOOST_LIBRARYDIR} AND
        DEFINED ENV{BOOST_INCLUDEDIR})
        set(BOOST_ROOT $ENV{BOOST_ROOT})
        file(TO_CMAKE_PATH "${BOOST_ROOT}" BOOST_ROOT)
        message(STATUS "Boost root: ${BOOST_ROOT}")

        set(BOOST_LIBRARYDIR $ENV{BOOST_LIBRARYDIR})
        file(TO_CMAKE_PATH "${BOOST_LIBRARYDIR}" BOOST_LIBRARYDIR)
        message(STATUS "Boost library directory: ${BOOST_LIBRARYDIR}")

        set(BOOST_INCLUDEDIR $ENV{BOOST_INCLUDEDIR})
        file(TO_CMAKE_PATH "${BOOST_INCLUDEDIR}" BOOST_INCLUDEDIR)
        message(STATUS "Boost include directory: ${BOOST_INCLUDEDIR}")
    else()
        message(FATAL_ERROR
            "Environment variable BOOST_ROOT, BOOST_LIBRARYDIR, "
            "or BOOST_INCLUDEDIR is not set.")
    endif()
endif()

find_package(Boost 1.80.0 REQUIRED system log log_setup REQUIRED)

if(Boost_FOUND)
    list(APPEND INCLUDE_DIR ${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost library not found.")
endif()

# Project configuration.
# Set the resource folder for the application.
set(RESOURCES_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

# Append the project include directory to the include directories.
list(APPEND INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/include/
)
include_directories(${INCLUDE_DIR})

# Set the source files.
file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Function to link the resources to the output directory.
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
    ${SOURCE_FILES}
)

# Set the C++ standard for the application.
set_target_properties(${APP_NAME} PROPERTIES
    CXX_STANDARD 20
)

# Link the application with the cocos2d library.
target_link_libraries(${APP_NAME} cocos2d ${Boost_LIBRARIES})

# Copy resources to the output directory.
link_resources(${APP_NAME})

# Copy the dynamic-link libraries on Windows.
if(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

# Unit testing configuration.
# Set the source files without main.cpp for unit testing.
set(SOURCE_FILES_WITHOUT_MAIN ${SOURCE_FILES})
list(REMOVE_ITEM SOURCE_FILES_WITHOUT_MAIN "${CMAKE_SOURCE_DIR}/src/main.cpp")

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
        gmock_main
        cocos2d
        ${Boost_LIBRARIES}
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
    add_googletest(utility logger_tests)
    add_googletest(server communication_tests)
    add_googletest(bag_gui bag_gui_tests)
    add_googletest(frontend talk_box_tests)
endif()
