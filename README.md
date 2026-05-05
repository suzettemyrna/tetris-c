# Tetris in C (ncurses)

A modular implementation of the classic Tetris game written in C using the ncurses library.
The project focuses on clean architecture, strict separation of concerns, and testability.

---

## Features

* Terminal-based UI (ncurses)
* Fully modular architecture (core / API / UI separation)
* Finite State Machine for game logic
* Static memory model (no dynamic allocation)
* Unit tests with mocks
* Configurable tetromino system
* Score tracking

---

## Architecture Overview

The project is divided into three main layers:

### 1. Core (Game Logic)

Responsible for all internal mechanics:

* game state management (FSM)
* collision detection
* field updates
* scoring system
* tetromino behavior

Key modules:

* `state_machine` — controls game flow
* `game_info` — central game state and data aggregation
* `field_control` — field validation and updates
* `tetromino` — piece representation and rotation logic

---

### 2. API Layer

Provides a minimal interface between the frontend and the backend.

```c
GameState_t getCurrentState();
GameInfo_t getCurrentGameInfo();
```

The frontend has **read-only access** to the game state.

---

### 3. CLI (Frontend)

Handles:

* rendering (ncurses)
* keyboard input
* user interaction

The UI is completely decoupled from the game logic.

---

## Game State Machine

The game is implemented as a finite state machine with the following states:

* START
* SPAWN
* MOVE
* SHIFT
* MERGE
* PAUSE
* GAME_OVER

![State Machine](docs/images/states.png)

---

## Controls

| Key   | Action                       |
| ----- | ---------------------------- |
| ← / → | Move piece                   |
| ↑     | Rotate piece                 |
| ↓     | Soft drop (accelerated fall) |
| Space | Pause                        |
| Enter | Start game                   |
| Esc   | Exit                         |

---

## Scoring System

| Lines Cleared | Points |
| ------------- | ------ |
| 1             | 100    |
| 2             | 300    |
| 3             | 700    |
| 4 (Tetris)    | 1500   |

* Level increases every 600 points
* Speed increases with level
* Max level is 10

---

## Build & Run

### Requirements

* gcc
* make
* ncurses
* check (for tests)

### Install dependencies (Ubuntu/Debian)

```bash
sudo apt-get install build-essential libncurses5-dev libncursesw5-dev check
```

### Build

```bash
make
make gui
```

### Run

```bash
./tetris
```

---

## Project Structure

```
src/
├── core/        # Game logic
├── api/         # Public interface
├── cli/         # Terminal UI
└── include/     # Headers
```

---

## Testing

```bash
make test
```

Unit tests include mocks for isolating modules.

---

## Future Improvements

* Wall kick system (SRS-like rotation)
* Improved input handling (true soft drop instead of instant drop)
* Enhanced Doxygen documentation
* Additional tetromino customization
* Persistent high score system (user directory)

---

## Design Notes

* Modules communicate only through defined interfaces
* The frontend cannot modify game state directly
* Emphasis on deterministic and testable logic
