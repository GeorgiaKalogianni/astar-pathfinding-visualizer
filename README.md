# A* Pathfinding Visualizer 

An interactive 2D application built in C++ with the SGG (Simple Game Graphics) library that visualizes how the A* algorithm finds the shortest path on a 15x25 grid.

<p align="center">
  <img width="600" height="375" src="https://github.com/user-attachments/assets/4c249890-60ff-4c7c-851c-f66021c22359" />
</p>
  
## What It Does
* Lets you draw and erase obstacles (walls) on the grid using the mouse.
* Allows you to move the Start and End points anywhere on the grid.
* Supports two modes: **Step-by-step animation** (with pause/resume) and **Instant pathfinding**.
* Includes simple sound effects for user actions.

## Controls
* **Left Click (and Drag):** Place walls/obstacles.
* **Right Click (and Drag):** Erase walls/obstacles.
* **`1` + Left Click:** Set/move Start Point (Green).
* **`2` + Left Click:** Set/move End Point (Red).
* **`Space` / `Enter`:** Start the A* pathfinding visualization/animation.
* **`S`:** Pause or resume the animation.
* **`R`:** Clear the grid and reset.

## Project Structure
* `main.cpp` – Application entry point and window setup.
* `GlobalState.h` / `GlobalState.cpp` – Manages the grid, A* algorithm, and user input.
* `Node.h` / `Node.cpp` – Represents each cell on the grid.
* `config.h` – Basic settings and constants.
* `assets/` – Audio files (`.mp3`) used for sound effects.

## How to Run
1. Download or clone this repository.
2. Open the source files in a C++ IDE (such as Visual Studio) configured with the **SGG library**.
3. Make sure the `assets/` folder is in the same directory as the executable, then build and run.
