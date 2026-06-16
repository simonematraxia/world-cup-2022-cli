# FIFA World Cup 2022 - Command Line Interface

**Course**: Fundamentals of Computer Science  
**Author**: Simone Giovanni Matraxia

## Overview
This repository contains a C program developed as a university project for the "Fundamentals of Computer Science" course. It provides a terminal-based ecosystem to track, simulate, and manage the FIFA World Cup Qatar 2022 dataset.

The system extracts data from two straightforward text files (containing the database of teams and matches played) and dynamically calculates statistics, standings, and historical data, displaying them through an interactive command-line interface.

## Features
* **User Authentication**: A local mock login system. While functionally optional for a simple stats tracker, it was specifically implemented as an academic exercise to demonstrate proficiency with **linked lists**, dynamic memory allocation (`malloc`), and file I/O parsing.
* **Data Extraction**: Automatic loading of group phases, matches, and knockout stages from text files.
* **Interactive Menus**: Browse through the Group Stage, Knockout Stage, and complete tournament statistics.
* **Dynamic Standings**: Real-time point calculation and bubble-sort algorithm implemented for group rankings.
* **Team Comparisons**: Compare performance metrics (Wins, Draws, Losses, Goal Difference) between multiple selected teams in normal or compact views.

## File Structure
* `main.c`: The core C application logic and UI.
* `Teams.txt`: Text-based database mapping 32 nations to their respective tournament groups.
* `Fixtures.txt`: Log of all 64 matches played, including penalty shootout results.
* `Login.txt`: Local mock database for user authentication credentials.

## How to Run
1. Ensure all `.txt` files and the `.c` source file are in the same directory.
2. Compile the source code using standard GCC:
   ```bash
   gcc main.c -o worldcup2022
   ```
3. Run the compiled executable:
   ```bash
   ./worldcup2022
   ```
