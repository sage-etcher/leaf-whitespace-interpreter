# Leaf Whitespace Interpretter (Nightly)
A System independant interpretter for the [whitespace programming language](https://en.wikipedia.org/wiki/Whitespace_%28programming_language%29)

LeafWSI is a C based interpretter for the esoteric programming language whitespace. The Interpretter takes in a whitespace file and executes the code in realtime. LeafWSI allows for customization of many key elements in how the program is interpretted and run. Customization can be done through the `src/config.h` file, some changes you can make include: custom stack length, recursion depth, the integer size in the stack/heap, how parameters are dealt with, and more!

This Interpretter assumes atleast general knowledge of how to compiling programs from source, and (for customization through config.h) basic knowledge of the C programming language.

# Builing & Installing the Interpretter
Instructions on how to compile/build and install the program on various systems. 

*Note: In order for changes to config.h to take effect, the build dirrectory must be clean; only then can you recompile the Interpretter. The build dirrectory can be cleaned by running `make clean build` in replacement of `make build`. The build dirrectory only needs cleaned for recompilations, the following instructions are for first time builds.

## Windows (MinGW)
Requires [MinGW](https://sourceforge.net/projects/mingw/files/) installed and added to Path
```
copy .\makefiles\Makefile.mingw .\Makefile
mingw32-make build
copy build\lwsi.exe [destination path]\lwsi.exe
```

## *nix (Linux/Unix/BSD)
Requires make (gmake on BSD) and a C compiler. If you wish to use a compiler other than GCC, you may need to specify `CC=[compiler]` and `LD=[linker]` when running make
```
cp ./makefiles/Makefile.linux ./Makefile
make build
sudo cp ./build/lwsi /usr/local/bin/
```

## Precompiled Binaries

If you would rather work with precompiled binaries over compiling from source, that is also an option. However, it is *not recomended* as you will be stuck using the default customization settings.

The Interpretter Binaries can be found *[here](https://github.com/sage-etcher/leaf-whitespace-interpretter/tree/binaries)*, if thats more up your alley.


# Using the Interpretter
The interpretter must have been built, installed, and the installation dirrectory must be in the Path variable, in order to use it

## Running a whitespace program
```
lwsi [whitespace file]<cr>
```


# File Descriptions
| File Name | Description |
| --- | --- |
| `config.h` | Contains configurable constants, such as the size of the stack. wsInt size, and other important customization. |
| `wsi.{c,h}` | The main file, contains main function and other large functions such as get_parameter, interpret_file, and run_program. |
| `ws_program.{c,h}` | Contains instruction definitions, functions, information, and the typedefs for important objects such as wsProgram, wsInstruction, wsInstructionDefinition, and the like. |
| `ws_error.{c,h}` | Contains error codes, strings, and error logging functions. |
| `hashmap.{c,h}` | Basic Hashmap module. Uses wsInt for both keys and values. Used for the runtime heap |
| `utils.{c,h}` | General ease of use functions and macros, such as MIN(a,b) and MAX(a,b) |

