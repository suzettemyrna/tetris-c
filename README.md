# Tetris (C, ncurses)

> A modular Tetris implementation in C focused on clean architecture, state-driven logic, and testability.

---

## Preview

![gameplay gif](docs/images/gameplay.gif)

---

## Purpose

This project was built to demonstrate:

* ability to design **modular architecture**
* clear separation between **core logic and interface**
* implementation of a **finite state machine (FSM)**
* writing **testable code in C**
* using **unit tests with mocks**
* building a complete CLI application from scratch

Although implemented in C, the same principles directly apply to higher-level development (e.g. Python).

---

## Overview

This is a terminal-based Tetris game built with **ncurses**.

The project is intentionally structured as a **layered system**, where:

* the **core game logic** is completely independent
* the **API layer** connects logic and interface
* the **CLI layer** handles rendering and user input only

No module has unnecessary knowledge about others.

---

## Architecture

[project architecture will be here]

### Key Design Decisions

* **Layered structure**

  * `core` — game logic
  * `api` — communication layer
  * `cli` — interface (ncurses)
  * `shared` — configuration

* **Strict boundaries**

  * no cyclic dependencies
  * controlled data access via API

* **State-driven logic**

  * the entire game is controlled by a finite state machine

* **Static memory model**

  * no dynamic allocation
  * predictable behavior

---

## Game State Machine

```mermaid
fsa_diagram-v2
    A[START] --> B[SPAWN]
    B --> C[MOVE]
    C --> D[MERGE]
    D --> E[GAME OVER]
    C <--> F[SHIFT]
    C <--> G[PAUSE]
    C --> E
    G --> E
    F --> D
    D --> B
    E --> A
```

Main states:

* `START`
* `SPAWN`
* `MOVE`
* `SHIFT` (continuous movement on key hold)
* `MERGE`
* `PAUSE`
* `GAME_OVER`

---

## Module Dependencies

[module dependencies will be here]

Dependency direction is strictly enforced:

```
cli → api → core
```

---

## Features

* Classic Tetris gameplay
* Terminal UI (ncurses)
* Score system with high score persistence
* Level progression (speed increases)
* Continuous movement on key hold
* Fully testable backend
* No global state leaks outside API

---

## Controls

| Key   | Action             |
| ----- | ------------------ |
| ← / → | Move left / right  |
| ↓     | Immidiate fall     |
| ↑     | Rotate piece       |
| SPACE | Pause / resume     |
| ENTER | Start game         |
| ESC   | Exit               |

---

## Scoring

* 1 line  → 100 points
* 2 lines → 300 points
* 3 lines → 700 points
* 4 lines → 1500 points

Level increases every **600 points**.

---

## Project Structure

```
src/
├── core/
├── api/
├── cli/
├── shared/
└── build/

tests/
```

---

## Build

### Requirements

* gcc
* make
* ncurses
* check (for tests)

### Commands

```bash
make            # build library
make gui        # build executable
make release    # full build
```

---

## Run

```bash
./build/bin/tetris
```

---

## Tests

```bash
make tests
```

* unit tests written with **Check**
* modules tested in isolation
* custom mocks for dependency control
* coverage report via lcov

---

## What this project demonstrates

This project reflects skills relevant for Python/backend development:

* system decomposition into independent modules
* API design between layers
* state machines for complex logic
* test isolation and mocking
* separation of business logic from UI
* predictable and maintainable code structure

---

## Limitations

* no wall-kick system for rotation (planned)
* soft drop behavior will be improved
* documentation can be expanded

---

## Future Improvements

* wall-kick implementation
* input system refinement
* improved documentation (Doxygen)
* extended gameplay features

---

## License

MIT License
