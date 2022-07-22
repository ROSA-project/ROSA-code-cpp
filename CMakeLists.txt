cmake_minimum_required(VERSION 3.10)

project (RosaSimulator VERSION 0.1)

# ==== Compiler flags ====

set(CMAKE_CXX_STANDARD 17) # gnu++-17
# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # Warn all
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # symbol info


# ==== Sanitizer ====

if (ADDR_SANITIZER GREATER 0)
    message(STATUS "--- ADDRESS SANITIZER: Enabled ---")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()


# ==== Common include path ==== 

include_directories("${PROJECT_BINARY_DIR}"
                    "src"
)


# ==== Source files ====

set(ROOT_SIM ${PROJECT_SOURCE_DIR}/src/simulator)

set(SIM_SRC
    ${ROOT_SIM}/shapes/cube.cpp
    ${ROOT_SIM}/shapes/cylinder.cpp
    ${ROOT_SIM}/intersection_instance.cpp
    ${ROOT_SIM}/object.cpp
    ${ROOT_SIM}/physical_object.cpp
    ${ROOT_SIM}/run_simulator.cpp
    ${ROOT_SIM}/world.cpp
)


add_executable(RosaSimulator ${SIM_SRC})

target_include_directories(RosaSimulator PUBLIC "src/simulator" )
