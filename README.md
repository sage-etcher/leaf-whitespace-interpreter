# Leaf Whitespace Interpretter (Nightly)

The nightly branch of the leaf whitespace interpretter. This is the quickest updated branch, however at the cost of stability. Some updates may not run, or may not run correctly, this branch is for development updates and for those that want the bleeding edge up to date version of the software. 

For the *majority* of people the [Stable branch](https://github.com/sage-etcher/leaf-whitespace-interpretter/tree/stable-dev) is recommended. 

For those that don't want to compile the program themselves, their is also a [Binary branch](https://github.com/sage-etcher/leaf-whitespace-interpretter/tree/binaries), which may or may or may not be up to date with the current nightly version.


# File Descriptions
| File Name | Description |
| --- | --- |
| `config.h` | Contains configurable constants, such as the size of the stack. wsInt size, and other important customization. |
| `wsi.{c,h}` | The main file, contains main function and other large functions such as get_parameter, interpret_file, and run_program. |
| `ws_program.{c,h}` | Contains instruction definitions, functions, information, and the typedefs for important objects such as wsProgram, wsInstruction, wsInstructionDefinition, and the like. |
| `ws_error.{c,h}` | Contains error codes, strings, and error logging functions. |
| `hashmap.{c,h}` | Basic Hashmap module. Uses wsInt for both keys and values. Used for the runtime heap |
| `utils.{c,h}` | General ease of use functions and macros, such as MIN(a,b) and MAX(a,b) |

