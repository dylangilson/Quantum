# Quantum

## Table of Contents
- [Quantum](#quantum-compiler)
- [Features](#features)
- [Syntax](#syntax)
- [Compilation and Running](#compilation-and-running)
- [Dependencies and C-Libraries](#dependencies-and-c-libraries)
- [Dependency Installation](#dependency-installation)
- [License](#license)

## Quantum Compiler
Quantum is a compiler for a custom C-style programming language

## Features
* Let and Const
* Addition, Subtraction, Multiplication, and Division
* If, Elif, and Else Scopes
* N-ary Expressions
* Single and Multi-line Comments
* Variable Reassignment
* Exit

## Syntax
* [Grammar](docs/grammar.md) is available in the docs folder
* Single-line comments begin with '//'
* Multi-line comments begin with '/\*' and end with '\*/'

## Compilation and Running
* change directory to the 'src' folder of the project, then running the following commands:

```sh
make
./quantum <input.qtm>
```

* IMPORTANT: Only runs on Linux Operating System Distributions

## Dependencies and C-Libraries
### Dependencies
* gcc
* nasm
* binutils (for ld linker)

### C-Libraries
* <stdio.h>
* <stdlib.h>
* <string.h>
* <ctype.h>

## Dependency Installation
* Debian Linux Distributions (e.g. Ubuntu):
	- install gcc, nasm, and binutils using commands: 

```sh
sudo apt install gcc
sudo apt install nasm
sudo apt install binutils
```

## License
Eliseo Copyright 2023
<br>
Code released under the [MIT License](LICENSE)
