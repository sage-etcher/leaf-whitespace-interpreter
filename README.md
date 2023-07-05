# LeafWSI
A System independant interpretter for the whitespace programming language

LeafWSI is a C based interpretter for the esoteric programming language whitespace. The Interpretter takes in a whitespace file and executes in realtime. Full customization of many key parts of the program, through the `src/config.h` file, allowing you to change the stacklength, max call depth, whitespace integer and more.

# Builing & Installing the Interpretter
Instructions on how to compile/build and install the program on various systems

## Windows (MinGW)
Requires GNU GCC installed and added to Path
```
mingw32-make -f makefiles\Makefile.mingw build
copy build\lwsi.exe [destination path]\lwsi.exe
```


## *nix (Linux/Unix/BSD)
Requires make (gmake on BSD) and a C compiler. If you don't want to use GCC you may have to specify `CC=[compiler]` and `LD=[linker]` when running make
```
make -f ./makefiles/makefile.linux build
sudo cp ./build/lwsi /usr/local/bin/
```

# Using the Interpretter
The program must have been built, installed, and the installation dirrectory must be in the Path variable
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

