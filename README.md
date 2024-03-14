# Minesweeper

This is a custom implementation of the classic Minesweeper game for Windows, built with C++ and the WinAPI for graphical interface rendering.

## Features

-   Classic Minesweeper gameplay.
-   Customizable difficulty levels (Beginner, Intermediate, Expert, and Custom).
-   Graphical user interface using WinAPI.
-   No external dependencies, pure WinAPI.
-   Timer and mine counter.

## Showcase

![Game view](https://github.com/MarcinSkrzypek/minesweeper/blob/main/assets/showcase.jpg "Game view")

## Getting Started

### Prerequisites

-   Windows OS.
-   [MinGW](https://www.mingw-w64.org/) or any compatible GCC for Windows.
-   [CMake](https://cmake.org/) (version 3.10 or newer).

### Building the Project

1. Clone the repository to your local machine.

    ```
    git clone https://github.com/MarcinSkrzypek/minesweeper.git
    ```

2. Navigate to the build directory in the project.

    ```
    cd minesweeper/build
    ```

3. Run CMake to configure the project and generate a Makefile.

    ```
    cmake -G "MinGW Makefiles" ..
    ```

4. Build the project with MinGW Make.

    ```
    mingw32-make
    ```

5. Run the game executable.

    ```
    ../minesweeper.exe
    ```

### Controls

-   Left-click on a cell to reveal it.
-   Right-click on a cell to mark it with "!" or "?".
-   Reset the game or change difficulty through the game menu.

## Used technologies

[<img align="left" width="32" height="32" alt="C++17" src="https://raw.githubusercontent.com/isocpp/logos/master/cpp_logo.png" style="padding: 0 32px 32px 0">](https://isocpp.org/get-started)
[<img align="left" width="32" height="32" alt="CMake" src="https://upload.wikimedia.org/wikipedia/commons/1/13/Cmake.svg" style="padding: 0 32px 32px 0">](https://cmake.org/)
