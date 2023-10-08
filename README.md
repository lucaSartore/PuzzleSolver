# Puzzle_Solver
This project consist in an algorithm that can solve real-world puzzles. as of right now the program is not
able to solve big puzzles, only small size one. And is intended to be used as a labeling tool to create a more
advanced version of a puzzle solver based on machine learning techniques.

## The project
An extensive paper has been written on this project, you can find it [here](LaTeX_paper/paper.pdf)


## Structure

The program is divided in 3 different components:
 - A ui made in c#
 - An Opencv based library that convert images into data made in C++
 - An algorithm that use the data provided by the C++ lib to find the solution made in rust

Everything is linked using dlls.

## Installation

### Requirements

The project right now is only compatible with Windows, and require to be compiled with CMake, and
the MSVC compiler. You will be able to run it on something else, with a bit of work...

### Opencv
This project need opencv `4.5.5` to run. you can download the precompiled (windows) version from
[here](https://opencv.org/releases/).  
One you do download it you have to:
 - and extract it
 - copy the content of the folder `build` to `your_path/PuzzleSolver/opencv/`
 - add `your_path/PuzzleSolver/opencv/x64/vc15/bin` to your system path
 
You can install it somewhere else if you what, you just need to change the `CMakeList.txt`  

Note that the version linked above is compiled with visual studio. using other compilers require another version

### System path
You need to add the folder `dll_output` to your system path.

### Installation
If you followed all the previous steps, and you have make install. the installation
is as simple opening a terminal inside `Puzzle_Solver ` and writing `make install`
Then you can build the target you want including:

 - `PuzzleSolverCommand`: create a file named `PuzzleSolver.exe` inside `dll_output`, that you can use to run
puzzle solver form the terminal
 - `PuzzleSolverLib`: create a file named `PuzzleSolverLib.dll` inside `dll_output`, that is used from the 
GUI in c# to work

## Usage

You can use the puzzle solver either in with the GUI (see solution inside folder C#_desktop_app) or from
the terminal using the command `PuzzleSolver` (assuming you have executed the target `PuzzleSolverCommand`)

## Limitations

Remember that for now the algorithm can solve only squared puzzles where the side is a power of two (2x2 4x4 8x8 16x16...)  

As already set the algorithm is not really fast...  you can expect it to take one minute to solve a 16 pieces puzzle, and
one hour to solve a 64 pieces puzzle.