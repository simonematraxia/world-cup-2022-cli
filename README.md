# FIFA World Cup 2022: CLI Database

**Course**: Fundamentals of Computer Science  
**Author**: Simone Giovanni Matraxia  

## Overview
This repository contains a C program developed as a free-choice academic project for the "Fundamentals of Computer Science" course. It provides a terminal-based ecosystem to track, simulate, and manage the complete FIFA World Cup Qatar 2022 dataset.

The system extracts data from local text files (acting as a raw database for teams and fixtures) and dynamically calculates statistics, standings, and historical match data, rendering them through an interactive Command-Line Interface (CLI).

## Core Features
* **User Authentication**: Implements a local mock login system. Built as a technical exercise to demonstrate proficiency with **Linked Lists**, dynamic memory allocation (`malloc`), and sequential file parsing.
* **Data Ingestion**: Automatic loading and parsing of group phases, match logs, and knockout stages from text files using C standard I/O libraries.
* **Dynamic Standings**: Real-time point calculation and custom implementation of a **Bubble-Sort algorithm** to generate accurate group rankings based on Points, Goal Difference, and Goals For.
* **Interactive Menus**: Browse through Group Stages, Knockout Stages, and complete tournament statistics.
* **Team Comparisons**: Extract and compare detailed performance metrics (Wins, Draws, Losses, Goal Difference) between multiple selected nations.

## File Structure
* `main.c`: The core C application logic containing data structures, memory management, and the terminal UI.
* `Teams.txt`: Text-based database mapping 32 nations to their respective tournament groups.
* `Fixtures.txt`: Chronological log of all 64 matches played, including regular time and penalty shootout results.
* `Login.txt`: Local mock database for user authentication credentials.

## Technical Stack
* **Language**: C
* **Environment**: Terminal / Command Line
* **Key Concepts**: Pointers, Linked Lists, Dynamic Memory Allocation, Bubble Sort, File I/O (`<stdio.h>`, `<stdlib.h>`)

## How to Build and Run
*Note: Ensure you have a C compiler (like GCC) installed on your system.*

1. Clone the repository and ensure all `.txt` files and the `main.c` source file are in the same directory.
```bash
git clone https://github.com/simonematraxia/c-world-cup-2022-db-terminal-interface.git
cd c-world-cup-2022-db-terminal-interface
```

2. Compile the source code using standard GCC:
```bash
gcc main.c -o worldcup2022
```

3. Run the compiled executable:
```bash
./worldcup2022
```