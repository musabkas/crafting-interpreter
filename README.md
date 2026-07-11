# Loxi - Lox Interpreter
This repository implements the Lox interpreter as described in Robert Nystorm's "Crafting Interpreters".

In this implementation, I have ported the Java code over to C++. With C++ being statically typed, I have made use of variant types which work C++17 onwards. Moreover, for effective garbage collection of objects declared on the heap, I make extensive use of smart pointers (unique and shared).

It was a pleasure to work on this! I hope this can encourage others to try something similar!

## To run
You can compile the interpreter with:
`make` <br>

To run a Lox program, write your script into `script.lox`<br>
Then run `make runscript` or `./loxi.out script.lox`

There is also prompt/REPL mode which is less reliable but can be started by `make runprompt` or `./loxi.out`

## How I am feeling rn
![legendary](legend.png)