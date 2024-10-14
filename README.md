# board_game

## Overview

This project simulates the spread of an infectious disease using the SIR (Susceptible, Infected, Recovered) model on a grid-based system. The simulation allows users to explore different configurations and visualize how an infection might propagate through a population represented on a grid.

## Features

- **Multiple Simulation Modes**: Users can choose from various preset scenarios or customize their own parameters.
- **Dynamic World Generation**: The grid is populated with different states (e.g., Susceptible, Infected, Blocked) based on user-defined probabilities.
- **Visual Output**: Utilizes SFML (Simple and Fast Multimedia Library) to provide graphical representation of the simulation.

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- SFML library installed

### Compiling the Project

To compile the project, use the following commands in your terminal:

```bash
g++ -c main.cpp
g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
./sfml-app
```

### Running the Simulation

Upon running the application, you will see a welcome message prompting you to select a simulation mode:

- **r**: Realistic Model
- **u**: Uncivilised Model
- **n**: Non-realistic Model
- **p**: Personalised Model
- **pa**: Personalised Advanced Model

Input your choice to start the simulation.

### Screenshot

Here is an example screenshot of one of the simulation modes:
![Example of a board state](https://github.com/nicmsri/board_game/blob/master/screenshots/option_r.png)

### Contributions

Contributions are welcome! Feel free to fork the repository and submit pull requests.
Enjoy exploring the dynamics of infectious disease spread!
