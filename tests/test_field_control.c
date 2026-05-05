#include "../src/core/field_control.c"
#include "include/test_helpers.h"

// =========================
// Tetramino-like test block

static int block[4][4] = {
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

// =====
// TESTS

START_TEST(test_field_init) {
  fieldInit();
  Field_t f = getCurrentFieldWithActiveTetromino();

  for (int y = 0; y < FIELD_HEIGHT; y++)
    for (int x = 0; x < FIELD_WIDTH; x++) ck_assert_int_eq(f.field[y][x], 0);

  ck_assert(!checkMerge());
}
END_TEST

START_TEST(test_update_field_simple) {
  fieldInit();
  updateFieldWithActiveTetromino(block, 0, 0);

  Field_t f = getCurrentFieldWithActiveTetromino();

  ck_assert_int_eq(f.field[0][0], 1);
  ck_assert_int_eq(f.field[0][1], 1);
  ck_assert_int_eq(f.field[1][0], 1);
  ck_assert_int_eq(f.field[1][1], 1);
}
END_TEST

START_TEST(test_can_place_ok) {
  fieldInit();
  ck_assert(canPlace(block, 0, 0));
}
END_TEST

START_TEST(test_can_place_out_of_bounds) {
  fieldInit();
  ck_assert(!canPlace(block, FIELD_WIDTH - 1, FIELD_HEIGHT - 1));
}
END_TEST

START_TEST(test_can_place_collision) {
  fieldInit();

  int block[4][4] = {0};
  block[0][0] = 1;

  canMoveDown(block, 5, FIELD_HEIGHT - 1);

  ck_assert(!canPlace(block, 5, FIELD_HEIGHT - 1));
}
END_TEST

START_TEST(test_can_move_right_blocked_by_wall) {
  fieldInit();
  ck_assert(!canMoveRight(block, FIELD_WIDTH - 2, 0));
}
END_TEST

START_TEST(test_can_move_left_blocked) {
  fieldInit();
  ck_assert(!canMoveLeft(block, 0, 0));
}
END_TEST

START_TEST(test_can_move_down_merge_bottom) {
  fieldInit();

  int y = FIELD_HEIGHT - 2;

  bool can = canMoveDown(block, 0, y);
  ck_assert(!can);
  ck_assert(checkMerge());
}
END_TEST

START_TEST(test_drop_tetromino) {
  fieldInit();
  resetMergeStatus();

  dropTetromino(block, 0, 0);

  ck_assert(checkMerge());
}
END_TEST

START_TEST(test_game_over_true) {
  fieldInit();

  extern Field_t WITHOUT_active_tetromino;
  for (int x = 0; x < FIELD_WIDTH; x++)
    WITHOUT_active_tetromino.field[0][x] = 1;

  ck_assert(checkGameOverCondition(block));
}
END_TEST

START_TEST(test_game_over_false) {
  fieldInit();
  ck_assert(!checkGameOverCondition(block));
}
END_TEST

START_TEST(test_clear_completed_lines) {
  fieldInit();

  extern Field_t WITHOUT_active_tetromino;
  for (int x = 0; x < FIELD_WIDTH; x++)
    WITHOUT_active_tetromino.field[FIELD_HEIGHT - 1][x] = 1;

  int lines = clearCompletedLines();
  ck_assert_int_eq(lines, 1);

  Field_t f = getCurrentFieldWithActiveTetromino();
  for (int x = 0; x < FIELD_WIDTH; x++)
    ck_assert_int_eq(f.field[FIELD_HEIGHT - 1][x], 0);
}
END_TEST

// =====
// SUITE

Suite *field_control_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("field_control");
  tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_field_init);
  tcase_add_test(tc_core, test_update_field_simple);

  tcase_add_test(tc_core, test_can_place_ok);
  tcase_add_test(tc_core, test_can_place_out_of_bounds);
  tcase_add_test(tc_core, test_can_place_collision);

  tcase_add_test(tc_core, test_can_move_right_blocked_by_wall);
  tcase_add_test(tc_core, test_can_move_left_blocked);

  tcase_add_test(tc_core, test_can_move_down_merge_bottom);
  tcase_add_test(tc_core, test_drop_tetromino);

  tcase_add_test(tc_core, test_game_over_true);
  tcase_add_test(tc_core, test_game_over_false);

  tcase_add_test(tc_core, test_clear_completed_lines);

  suite_add_tcase(s, tc_core);

  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite *s = field_control_suite();
  SRunner *sr = srunner_create(s);

  printf("Running user_actions tests...\n");
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All field_control tests passed\n\n");
  else
    printf("Field_control tests failed: %d\n\n", failed);

  return failed;
}