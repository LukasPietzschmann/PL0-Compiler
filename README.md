# PL0-Compiler
A compiler for the minimal programming language PL/0 written in C++

## Compile the project
To compile the code yourself, execute the following steps:

1. Navigate to the projects root
2. Create a new folder (e.g. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCMAKE_BUILD_TYPE=Release`
4. Now you can build the project by calling `make main`. The resulting executable can be found under `./bin`
