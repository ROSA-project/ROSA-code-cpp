# ROSA-code-cpp
Robot Simulation Architecture, in C++.

## Requirements
1. Compiler:
   * GCC version 8,
   * or, Clang version 5 (not tested)
   * or, MSVC 19.15 (not tested)
2. Build tool:
   * CMake version 3.10

## How to Build
### 1. Fetch external dependencies:
```./prepare.sh -j8```

Temporarily bypassed by adding the dependencies to the repository as downloading from the Internet has to be made portable. To be fixed.

### 2. Build the project:
```
mkdir build
cd build
cmake ../
make -j8
```

## How to Run
In `build` folder, run the executable as below:
```
./RosaSimulator -m ../sample_maps/multiple_walls_1ball.json
```
