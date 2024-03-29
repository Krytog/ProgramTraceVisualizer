# Instruction pointer capturing via clang coverage

If there's access to the source code of the target program, its trace can be captured via clang coverage: https://clang.llvm.org/docs/SanitizerCoverage.html. This directory contains relevant utilities.

## Usage 
* The target program should be compiled with clang. Usually, it's enough to add ```set(CMAKE_CXX_COMPILER clang++)``` and ```set(CMAKE_C_COMPILER clang)``` before ```project()``` call in the CMakeLists.txt of the target.
* Add ```set(CMAKE_CXX_FLAGS -fsanitize-coverage=func,trace-pc-guard)``` (or equivalent). Notice that it's possible to choose targets to which coverage is added
* Add this directory as a subdirectory in the CMakeLists.txt of the target: ```add_subdirectory(<path_to_this_folder>)```.
* Build the target.
* On success, when the target runs, a ```captured_ip.trace``` file is created, into which the trace is printed.