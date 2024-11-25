# Quantum

## Table of Contents
- [Quantum](#quantum-compiler)
- [Features](#features)
- [Syntax](#syntax)
- [Compilation and Running](#compilation-and-running)
- [Dependencies](#dependencies)
- [License](#license)

## Quantum Compiler
Quantum is a compiler for a custom C-like programming language

## Features
* Let and Const
* Addition, Subtraction, Multiplication, and Division
* If, Elif, and Else Scopes
* n-ary Expressions
* Single and Multi-line Comments
* Variable Reassignment
* Exit

## Syntax
[Grammar](docs/grammar.md) is available in the docs folder
Single-line comments are written as //
Multi-line comments begin with '/\*' and end with '\*/'

## Compilation and Running
* change directory to the 'src' folder of the project, then running the following commands:

```sh
make
./quantum <input.qtm>
```

## Dependencies
* <stdio.h>
* <stdlib.h>
* <string.h>
* <ctype.h>

## License
Eliseo Copyright 2023
<br>
Code released under the [MIT License](LICENSE)
