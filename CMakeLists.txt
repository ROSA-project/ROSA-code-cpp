cmake_minimum_required(VERSION 3.10)

project (RosaSimulator VERSION 0.1)

# ==== Compiler flags ====
set(CMAKE_CXX_STANDARD 17) # gnu++-17
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # produces compile_commands.json

if (MSVC)
    # Use /MT for static runtime or /MD for dynamic runtime
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")  # or /MD
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")  # or /MDd
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /Zi")
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Clang-specific flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")
    else()
        # Other compilers (e.g., GCC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
    endif()
endif()

# ==== Sanitizer ====
if (NOT MSVC)
    if (ADDR_SANITIZER GREATER 0)
        message(STATUS "--- ADDRESS SANITIZER: Enabled ---")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    endif()
endif()

# Check for clang and set specific flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using Clang compiler")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")
endif()
# ==== Clang tidy ====

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)

if(NOT CLANG_TIDY_COMMAND)
    message(WARNING "Could not find clang-tidy!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE)
else()
    message(STATUS "Found clang-tidy")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_CLANG_TIDY
        clang-tidy;
        -header-filter=src/.; # this excludes checking external libs
        # -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy;
        -format-style=file;
        -p ${CMAKE_BINARY_DIR};
        # -warnings-as-errors=*;  # not too harsh for now : )
    )
endif()

# ==== Eigen3 ====
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})
# Link Eigen3 to the targets


# === GoogleTest ===

# Add GoogleTest as a subdirectory
add_subdirectory(external_deps/googletest)

# Include GoogleTest headers
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# ==== Common include path ==== 

include_directories("${PROJECT_BINARY_DIR}"
                    "src"
                    "external_deps/include"
)


# ==== Source files ====

set(ROOT_SIM ${PROJECT_SOURCE_DIR}/src/simulator)

set(E2E_TEST_SRC
    ${PROJECT_SOURCE_DIR}/src/e2e-tests/freespace_motion_test.cpp    
)

set(UNIT_TEST_SRC
    ${PROJECT_SOURCE_DIR}/src/unit-tests/test_intersection.cpp
)

set(SIM_SRC
    ${ROOT_SIM}/ball.cpp
    ${ROOT_SIM}/cube.cpp
    ${ROOT_SIM}/cylinder.cpp
    ${ROOT_SIM}/sphere.cpp
    ${ROOT_SIM}/intersection_instance.cpp
    ${ROOT_SIM}/map.cpp
    ${ROOT_SIM}/object.cpp
    ${ROOT_SIM}/rigid_physical_obj.cpp
    ${ROOT_SIM}/vacuum_cleaner.cpp
    ${ROOT_SIM}/world.cpp
    ${PROJECT_SOURCE_DIR}/src/common/util.cpp
    ${PROJECT_SOURCE_DIR}/src/config.cpp
)

enable_testing()

add_executable(RosaSimulator ${SIM_SRC} ${ROOT_SIM}/run_simulator.cpp)
add_executable(E2E_tests ${E2E_TEST_SRC} ${SIM_SRC})
add_executable(Unit_tests ${UNIT_TEST_SRC} ${SIM_SRC})

target_include_directories(RosaSimulator PUBLIC "src/simulator" )
target_include_directories(E2E_tests PUBLIC "src/simulator" )
target_include_directories(Unit_tests PUBLIC "src/simulator" )

target_link_libraries(E2E_tests gtest_main)
target_link_libraries(Unit_tests gtest_main)

target_link_libraries(RosaSimulator Eigen3::Eigen)
target_link_libraries(E2E_tests Eigen3::Eigen)
target_link_libraries(Unit_tests Eigen3::Eigen)

include(GoogleTest)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Building on Linux or macOS")
    gtest_discover_tests(E2E_tests)
    gtest_discover_tests(Unit_tests)
endif()


