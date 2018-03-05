# **COSMOS** _CHIP-8 Interpreter_

[![Build Status](https://travis-ci.org/LightningWright/cosmos.svg?branch=master)](https://travis-ci.org/LightningWright/cosmos)

### Work In Progress.

cosmos is a CHIP-8 VM Interpreter developed in C++. SDL2 is used to render the screen and emulate sound.

## Building from sources

### Linux

In order to compile this project, you will need the following dependencies:

- libsdl2-dev
- cmake

You can install this dependencies typing the following command:

`  $ apt-get install libsdl2-dev cmake -y `

Once you have done these steps you will be able to compile the program.
It's very simple to compile, you just have to type:

```
$ cmake .
$ make
```

Now you can `ls` to see the binary that has been generated right now.

### Windows

You will need to set up MinGW or any tool that gives you the ability to compile the program following the same steps as in Linux.

I personally use Microsoft Visual Studio 2017. The steps you have to follow are simple.
- Download [SDL2 library](www.libsdl.org)
- Unzip and copy the contents
- Configure the solution (I've included "cosmos.vcxproj", you only have to change a few settings)
- Press F7 to compile the solution.

##### Warning! You need to copy SDL2.dll to the same directory as the executable. Otherwise it will crash.

## Usage


```sh
$ ./cosmos "path_to_rom"
```

* * *

I have learnt a lot with this project. Also i want to keep updating the project to help others to create their first interpreter/emulator. If there is anything that has to be changed in order to understand better, feel free to contact me, or open a issue.


* * *

#### Coded with 💜 by ![](https://avatars3.githubusercontent.com/u/6288559?v=2&s=32) [LightningWright](https://github.com/LightningWright)
