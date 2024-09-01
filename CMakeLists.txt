cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")


project (RosaSimulator VERSION 0.1)

# ==== Compiler flags ====
set(CMAKE_CXX_STANDARD 17) # gnu++-17
# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # Warn all
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # symbol info
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # produces compile_commands.json


# ==== Sanitizer ====

if (ADDR_SANITIZER GREATER 0)
    message(STATUS "--- ADDRESS SANITIZER: Enabled ---")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
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


# ==== Common include path ==== 

include_directories("${PROJECT_BINARY_DIR}"
                    "src"
                    "external_deps/include"
)


# ==== Source files ====

set(ROOT_SIM ${PROJECT_SOURCE_DIR}/src/simulator)

set(SIM_SRC
    ${ROOT_SIM}/ball.cpp
    ${ROOT_SIM}/cube.cpp
    ${ROOT_SIM}/cylinder.cpp
    ${ROOT_SIM}/intersection_instance.cpp
    ${ROOT_SIM}/map.cpp
    ${ROOT_SIM}/object.cpp
    ${ROOT_SIM}/rigid_physical_obj.cpp
    ${ROOT_SIM}/run_simulator.cpp
    ${ROOT_SIM}/vacuum_cleaner.cpp
    ${ROOT_SIM}/world.cpp
    ${PROJECT_SOURCE_DIR}/src/common/util.cpp
    ${PROJECT_SOURCE_DIR}/src/config.cpp
)

add_executable(RosaSimulator ${SIM_SRC})

target_include_directories(RosaSimulator PUBLIC "src/simulator" )