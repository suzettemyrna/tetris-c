/**
 * @file state_machine.h
 * @brief Finite State Machine interface for the Tetris game.
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../../game_model/game_model.h"
#include "backend_objects.h"

/**
 * @brief Represents the current state of the state machine.
 *
 * This structure is intentionally minimal and exposed for read-only purposes.
 */
typedef struct {
  GameState_t state; /**< Current game state */
} StateMachine;

/**
 * @brief Get current control flags.
 *
 * @return ControlFlags_t structure
 */
ControlFlags_t getGameControlFlags();

// GameState_t getCurrentState(); // declared in tetris_api.h

/**
 * @brief Check and reset first iteration flag.
 *
 * @return true if first iteration
 */
bool isFirstIteration();

/**
 * @brief Process FSM transition.
 *
 * @param action User action
 * @param hold Whether key is held
 */
void handleStates(UserAction_t action, bool hold);

#endif  // STATE_MACHINE_H