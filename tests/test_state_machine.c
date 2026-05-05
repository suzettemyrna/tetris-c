#include "../src/core/state_machine.c"
#include "include/test_helpers.h"

// =======
// Helpers

static void reset_state_machine_for_test(void) {
  current.state = STATE_START;
  flags.first_iteration = TRUE;
  flags.game_finished = FALSE;
  flags.exit_required = FALSE;
}

static void mock_set_state(GameState_t s) { current.state = s; }

// =====
// TESTS

START_TEST(tc_start_to_spawn) {
  reset_state_machine_for_test();
  mock_set_SM_checkMerge_return(FALSE);
  mock_set_SM_checkGameOver_return(FALSE);

  handleStates(ACTION_START, FALSE);

  ck_assert_int_eq(getCurrentState(), STATE_SPAWN);
  ControlFlags_t f = getGameControlFlags();
  ck_assert_msg(f.first_iteration == TRUE || f.first_iteration == FALSE,
                "flags.first_iteration must be boolean");
  ck_assert_msg(f.game_finished == FALSE,
                "game_finished must be false after start");
}
END_TEST

START_TEST(tc_start_to_exit) {
  reset_state_machine_for_test();

  handleStates(ACTION_TERMINATE, FALSE);

  ck_assert_int_eq(getCurrentState(), STATE_START);
  ControlFlags_t f = getGameControlFlags();
  ck_assert_msg(f.exit_required == TRUE,
                "exit_required should be TRUE after ACTION_TERMINATE in START");
}
END_TEST

START_TEST(tc_spawn_to_move) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_SPAWN);

  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);
}
END_TEST

START_TEST(tc_move_basic) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);

  mock_set_SM_checkMerge_return(FALSE);
  handleStates(ACTION_NONE, FALSE);

  ck_assert_int_eq(getCurrentState(), STATE_MOVE);
}
END_TEST

START_TEST(tc_move_merged) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);

  mock_set_SM_checkMerge_return(TRUE);
  handleStates(ACTION_NONE, FALSE);

  ck_assert_int_eq(getCurrentState(), STATE_MERGE);
}
END_TEST

START_TEST(tc_move_shift) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);

  mock_set_SM_checkMerge_return(FALSE);
  handleStates(ACTION_LEFT, TRUE);

  ck_assert_int_eq(getCurrentState(), STATE_SHIFT);
}
END_TEST

START_TEST(tc_shift_to_move) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  mock_set_SM_checkMerge_return(FALSE);
  handleStates(ACTION_LEFT, TRUE);
  ck_assert_int_eq(getCurrentState(), STATE_SHIFT);

  mock_set_SM_checkMerge_return(FALSE);
  handleStates(ACTION_LEFT, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);
}
END_TEST

START_TEST(tc_merge_to_gameover) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  mock_set_SM_checkMerge_return(TRUE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MERGE);

  mock_set_SM_checkGameOver_return(TRUE);
  handleStates(ACTION_NONE, FALSE);

  ck_assert_int_eq(getCurrentState(), STATE_GAME_OVER);
}
END_TEST

START_TEST(tc_merge_to_spawn) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  mock_set_SM_checkMerge_return(TRUE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MERGE);

  mock_set_SM_checkGameOver_return(FALSE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_SPAWN);
}
END_TEST

START_TEST(tc_pause_to_move_and_back) {
  reset_state_machine_for_test();

  handleStates(ACTION_START, FALSE);
  handleStates(ACTION_NONE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);

  handleStates(ACTION_PAUSE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_PAUSE);

  handleStates(ACTION_PAUSE, FALSE);
  ck_assert_int_eq(getCurrentState(), STATE_MOVE);
}
END_TEST

START_TEST(tc_gameover_finish) {
  mock_set_state(STATE_GAME_OVER);

  handleStates(ACTION_TERMINATE, FALSE);

  ControlFlags_t f = getGameControlFlags();
  ck_assert(f.game_finished);
  ck_assert(f.exit_required);
}
END_TEST

// =====
// SUITE

Suite *state_machine_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("state_machine");
  tc_core = tcase_create("core");

  tcase_add_test(tc_core, tc_start_to_spawn);
  tcase_add_test(tc_core, tc_start_to_exit);
  tcase_add_test(tc_core, tc_spawn_to_move);

  tcase_add_test(tc_core, tc_move_basic);
  tcase_add_test(tc_core, tc_move_merged);
  tcase_add_test(tc_core, tc_move_shift);
  tcase_add_test(tc_core, tc_shift_to_move);
  tcase_add_test(tc_core, tc_merge_to_gameover);
  tcase_add_test(tc_core, tc_merge_to_spawn);
  tcase_add_test(tc_core, tc_pause_to_move_and_back);
  tcase_add_test(tc_core, tc_gameover_finish);

  suite_add_tcase(s, tc_core);

  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite *s = state_machine_suite();
  SRunner *sr = srunner_create(s);

  printf("Running state_machine tests...\n");
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All state_machine tests passed\n\n");
  else
    printf("State_machine tests failed: %d\n\n", failed);

  return failed;
}