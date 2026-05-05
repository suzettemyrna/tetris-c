#include "../src/core/user_actions.c"
#include "include/test_helpers.h"

// =====
// TESTS

START_TEST(test_initCurrPlayerName) {
  initCurrPlayerName();

  for (int i = 0; i < MAX_NAME_LENGTH; i++)
    ck_assert_int_eq(curr_player_name[i], ' ');

  ck_assert_int_eq(curr_player_name[MAX_NAME_LENGTH], '\0');
}
END_TEST

START_TEST(test_handlePlayerNameInput_add) {
  initCurrPlayerName();

  handlePlayerNameInput('A');
  handlePlayerNameInput('b');
  handlePlayerNameInput('3');

  ck_assert_int_eq(curr_player_name[0], 'A');
  ck_assert_int_eq(curr_player_name[1], 'b');
  ck_assert_int_eq(curr_player_name[2], '3');
}
END_TEST

START_TEST(test_handlePlayerNameInput_backspace) {
  initCurrPlayerName();

  handlePlayerNameInput('X');
  handlePlayerNameInput('Y');
  handlePlayerNameInput(KEY_BACKSPACE);

  ck_assert_int_eq(curr_player_name[0], 'X');
  ck_assert_int_eq(curr_player_name[1], ' ');
}
END_TEST

START_TEST(test_playerNameValid) {
  initCurrPlayerName();
  ck_assert(!playerNameValid());

  handlePlayerNameInput('Z');
  ck_assert(playerNameValid());
}
END_TEST

START_TEST(test_mapKeyToAction) {
  ck_assert_int_eq(mapKeyToAction(KEY_LEFT), ACTION_LEFT);
  ck_assert_int_eq(mapKeyToAction(KEY_RIGHT), ACTION_RIGHT);
  ck_assert_int_eq(mapKeyToAction(KEY_DOWN), ACTION_DOWN);
  ck_assert_int_eq(mapKeyToAction(KEY_UP), ACTION_ACTION);
  ck_assert_int_eq(mapKeyToAction(' '), ACTION_PAUSE);
  initCurrPlayerName();
  ck_assert_int_eq(mapKeyToAction(ENTER_KEY),
                   ACTION_NONE);  // invalid cause: no name
}
END_TEST

START_TEST(test_handleInput_start_state) {
  initCurrPlayerName();
  setState(STATE_START);

  handleInput('Q');

  ck_assert_int_eq(curr_player_name[0], 'Q');  // name is written
}
END_TEST

START_TEST(test_handleInput_other_state) {
  initCurrPlayerName();
  setState(STATE_MOVE);

  handleInput('Z');

  ck_assert_int_eq(curr_player_name[0], ' ');  // name hasn't changed
}
END_TEST

// =====
// SUITE

Suite *user_actions_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("user_actions");
  tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_initCurrPlayerName);
  tcase_add_test(tc_core, test_handlePlayerNameInput_add);
  tcase_add_test(tc_core, test_handlePlayerNameInput_backspace);
  tcase_add_test(tc_core, test_playerNameValid);
  tcase_add_test(tc_core, test_mapKeyToAction);
  tcase_add_test(tc_core, test_handleInput_start_state);
  tcase_add_test(tc_core, test_handleInput_other_state);

  suite_add_tcase(s, tc_core);

  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite *s = user_actions_suite();
  SRunner *sr = srunner_create(s);

  printf("Running user_actions tests...\n");
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All user_actions tests passed\n\n");
  else
    printf("User_actions tests failed: %d\n\n", failed);

  return failed;
}