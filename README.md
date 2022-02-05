# PL0-Compiler
A compiler and interpreter for the minimal programming language PL/0 written in C++.

The parser uses the grammar from [Wikipedia](https://en.wikipedia.org/wiki/PL/0#Grammar)

The compiler targets a custom instruction set. Therefore the project also builds an assembler and vm to execute the generated code.

## Compile the project
To compile the code yourself, execute the following steps:

1. Navigate to the projects root
2. Create a new folder (e.g. Debug) with `mkdir Debug && cd Debug`
3. Within this folder, issue the command `cmake .. -DCMAKE_BUILD_TYPE=Release`
4. Now you have to decide which target to build
   - `pl0`: this only builds the interpreter
   - `pl0c`: this builds the compiler, assembler and vm
   - `compile_and_run_example`: this is a handy target, if you want to execute the generated asm code directly. This target assumes you have a file called `example.pl0` in the projects root. To use another file set the cmake variabel `EXAMPLE_FILE` to the filepath you want to use
5. Now you can run all build executables located in the `./bin` folder
6. All executables take a file as their first command line argument. Image you have a `hello.pl0` file, then you would invoke the compiler, interpreter, assembler, or vm with `<executable> hello`