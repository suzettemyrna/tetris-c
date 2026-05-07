#include "../src/game_loop/game_loop.c"
#include "include/test_helpers.h"

// =========
// Variables

extern int mock_cli_init_calls;
extern int mock_cli_end_calls;
extern int mock_drawingManager_calls;
extern int mock_initGameInfo_calls;
extern int mock_resetGameInfo_calls;
extern int mock_cleanGameInfo_calls;
extern int mock_initCurrPlayerName_calls;
extern int mock_handleStates_calls;
extern UserAction_t mock_handleInput_return;

// =====
// TESTS

START_TEST(tc_init_called) {
  mock_cli_reset();
  mock_state_reset();
  mock_set_iteration_limit(3);

  tetrisGameLoop();

  ck_assert_msg(mock_cli_init_calls >= 1, "cliInit not called");
  ck_assert_msg(mock_initCurrPlayerName_calls >= 1,
                "initCurrPlayerName not called");
  ck_assert_msg(mock_initGameInfo_calls >= 1, "initGameInfo not called");
}
END_TEST

START_TEST(tc_reset_only_on_first_iteration) {
  mock_cli_reset();
  mock_state_reset();
  mock_set_iteration_limit(4);

  mock_set_state_sequence(
      (GameState_t[]){STATE_MOVE, STATE_MOVE, STATE_MOVE, STATE_MOVE}, 4);

  tetrisGameLoop();

  ck_assert_msg(
      mock_resetGameInfo_calls == 1,
      "resetGameInfo must be called exactly once (on first iteration)");
}
END_TEST

START_TEST(tc_handleStates_when_action_from_input) {
  mock_cli_reset();
  mock_state_reset();
  mock_set_iteration_limit(4);

  // Provide repeated input; mocked handleInput always returns ACTION_LEFT
  int seq[] = {'A', 'A', 'A', 'A'};
  mock_set_input_sequence(seq, 4);
  mock_handleInput_return = ACTION_LEFT;

  tetrisGameLoop();

  ck_assert_msg(mock_handleStates_calls >= 1,
                "handleStates should be called when user action present");
}
END_TEST

START_TEST(tc_handleStates_on_spawn_state) {
  mock_cli_reset();
  mock_state_reset();
  mock_set_iteration_limit(4);

  int seq[] = {NO_INPUT, NO_INPUT, NO_INPUT, NO_INPUT};
  mock_set_input_sequence(seq, 4);

  mock_set_state_sequence(
      (GameState_t[]){STATE_SPAWN, STATE_MOVE, STATE_MOVE, STATE_MOVE}, 4);

  tetrisGameLoop();

  ck_assert_msg(mock_handleStates_calls >= 1,
                "handleStates should be called for STATE_SPAWN");
}
END_TEST

START_TEST(tc_cleanup_called) {
  mock_cli_reset();
  mock_state_reset();
  mock_set_iteration_limit(2);

  tetrisGameLoop();

  ck_assert_msg(mock_cli_end_calls == 1, "cliEnd should be called once");
  ck_assert_msg(mock_cleanGameInfo_calls == 1,
                "cleanGameInfo should be called once");
}
END_TEST

// =====
// SUITE

Suite *tetris_api_suite(void) {
  Suite *s = suite_create("tetris_api");
  TCase *tc_core = tcase_create("core");

  tcase_add_test(tc_core, tc_init_called);
  tcase_add_test(tc_core, tc_reset_only_on_first_iteration);
  tcase_add_test(tc_core, tc_handleStates_when_action_from_input);
  tcase_add_test(tc_core, tc_handleStates_on_spawn_state);
  tcase_add_test(tc_core, tc_cleanup_called);

  suite_add_tcase(s, tc_core);
  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite *s = tetris_api_suite();
  SRunner *sr = srunner_create(s);

  printf("Running tetris_api tests...");
  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All tetris_api tests passed \n\n");
  else
    printf("Tetris_api tests failed: %d\n\n", failed);

  return failed;
}