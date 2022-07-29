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
)


# find_package(nlohmann_json 3.10.5 REQUIRED)


add_executable(RosaSimulator ${SIM_SRC})

target_include_directories(RosaSimulator PUBLIC "src/simulator" )

# target_link_libraries(RosaSimulator PRIVATE nlohmann_json::nlohmann_json)
