# Leaf Whitspace Interpreter
A System independant interpreter for the [whitespace programming language](https://en.wikipedia.org/wiki/Whitespace_%28programming_language%29)

LeafWSI is a C based interpreter for the esoteric programming language whitespace. The Interpreter takes in a whitespace file and executes the code in realtime. LeafWSI allows for customization of many key elements in how the program is interpreted and run. Customization can be done through the `src/config.h` file, some changes you can make include: custom stack length, recursion depth, the integer size in the stack/heap, how parameters are dealt with, and more!

This Interpreter assumes atleast general knowledge of compiling programs from source, and (for customization through config.h) basic knowledge of the C programming language.

# Builing & Installing the Interpreter
Instructions on how to compile/build and install the program on various systems. 

*Note: In order for changes to config.h to take effect, the build dirrectory must be clean; only then can you recompile the Interpreter. The build dirrectory can be cleaned by running `make clean build` in replacement of `make build`. The build dirrectory only needs cleaned for recompilations, the following instructions are for first time builds.

## Windows (MinGW)
Requires [MinGW](https://sourceforge.net/projects/mingw/files/) installed and added to Path

### Building
Builds the object files and executable in BUILD_DIR
  
```
copy .\makefiles\Makefile.mingw .\Makefile
mingw32-make build
```

### Installing (optional)
Installs the program in INSTALL_DIR; default installation path `%appdata%/leaf-whitespace-interpreter/[version]`. 

Please note that this does **not** add the program to your PATH varibale; this must be done manually. Instructions can be found *[here](https://windowsloop.com/how-to-add-to-windows-path/)* if you require assistance.

```
mingw32-make install
```

## *nix (Linux/Unix/BSD)
Requires make (gmake on BSD) and a C compiler. 

### Building
If you are using a compiler other than GCC, you may need to specify `CC=[compiler]` and `LD=[linker]` when building with make 

```
cp ./makefiles/Makefile.linux ./Makefile
make build
```

### Installing
Installs the program in INSTALL_DIR; default installation path is `/usr/local/bin`.
as sudo 

``` 
make install
```

# Binary Releases
If you prefer pre-compiled binaries you can find them *[here](https://github.com/sage-etcher/leaf-whitespace-interpreter/releases/tag/v1.0.1)*. However, please note that the binaries offer less customizability.

# Using the Interpreter
The interpreter must have been built, installed, and the installation dirrectory must be in the Path variable, in order to use it

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

