#include "../src/core/tetromino.c"
#include "include/test_helpers.h"

// ================
// Tetromino shapes

static const int SHAPE_I[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
static const int SHAPE_J[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {2, 0, 0, 0}, {2, 2, 2, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
static const int SHAPE_L[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 0, 3, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
static const int SHAPE_T[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 3, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
static const int SHAPE_O[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 4, 4, 0}, {0, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
static const int SHAPE_S[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {0, 5, 5, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
static const int SHAPE_Z[TETROMINO_SIZE][TETROMINO_SIZE] = {
    {6, 6, 0, 0}, {0, 6, 6, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

// ======
// Helper

static int shapes_equal(const int a[TETROMINO_SIZE][TETROMINO_SIZE],
                        const int b[TETROMINO_SIZE][TETROMINO_SIZE]) {
  int res = 1;
  for (int y = 0; y < TETROMINO_SIZE; y++)
    for (int x = 0; x < TETROMINO_SIZE; x++)
      if (a[y][x] != b[y][x]) res = 0;
  return res;
}

// =====
// TESTS

START_TEST(tc_detect_all_types) {
  ck_assert_int_eq(detectTetromino(SHAPE_I), TETROMINO_I);
  ck_assert_int_eq(detectTetromino(SHAPE_J), TETROMINO_J);
  ck_assert_int_eq(detectTetromino(SHAPE_L), TETROMINO_L);
  ck_assert_int_eq(detectTetromino(SHAPE_T), TETROMINO_T);
  ck_assert_int_eq(detectTetromino(SHAPE_O), TETROMINO_O);
  ck_assert_int_eq(detectTetromino(SHAPE_S), TETROMINO_S);
  ck_assert_int_eq(detectTetromino(SHAPE_Z), TETROMINO_Z);
}
END_TEST

START_TEST(tc_spawn_sets_type_and_position) {
  mock_field_control_reset();

  spawnTetromino((int(*)[4])SHAPE_T);
  ActiveTetromino_t info = getCurrentTetrominoInfo();

  ck_assert_int_eq(info.base.type, TETROMINO_T);
  ck_assert_int_eq(info.x, TETROMINO_SPAWN_X);
  ck_assert_int_eq(info.y, TETROMINO_SPAWN_Y);

  ck_assert_int_eq(shapes_equal(info.base.shape, SHAPE_T), 1);
}
END_TEST

START_TEST(tc_move_left_right_down_and_drop) {
  mock_field_control_reset();

  spawnTetromino((int(*)[4])SHAPE_I);
  ActiveTetromino_t before = getCurrentTetrominoInfo();

  mock_set_canMoveLeft(1);
  tetrominoMovingManager(ACTION_LEFT);
  ActiveTetromino_t after_left = getCurrentTetrominoInfo();
  ck_assert_int_eq(after_left.x, before.x - 1);

  before = getCurrentTetrominoInfo();
  mock_set_canMoveRight(1);
  tetrominoMovingManager(ACTION_RIGHT);
  ActiveTetromino_t after_right = getCurrentTetrominoInfo();
  ck_assert_int_eq(after_right.x, before.x + 1);

  before = getCurrentTetrominoInfo();
  mock_set_canMoveDown(1);
  tetrominoMovingManager(ACTION_NONE);
  ActiveTetromino_t after_down = getCurrentTetrominoInfo();
  ck_assert_int_eq(after_down.y, before.y + 1);

  int drops_before = mock_get_drop_called();
  tetrominoMovingManager(ACTION_DOWN);
  ck_assert_int_ge(mock_get_drop_called(), drops_before + 1);
}
END_TEST

START_TEST(tc_rotate_non_O_and_O_behaviour) {
  mock_field_control_reset();

  spawnTetromino((int(*)[4])SHAPE_J);
  ActiveTetromino_t before = getCurrentTetrominoInfo();
  int oldshape[TETROMINO_SIZE][TETROMINO_SIZE];
  memcpy(oldshape, before.base.shape, sizeof(oldshape));

  mock_set_canPlace(1);
  tetrominoMovingManager(ACTION_ACTION);
  ActiveTetromino_t after = getCurrentTetrominoInfo();

  ck_assert_int_eq(shapes_equal(oldshape, after.base.shape), 0);

  spawnTetromino((int(*)[4])SHAPE_O);
  ActiveTetromino_t o_before = getCurrentTetrominoInfo();
  mock_set_canPlace(1);
  tetrominoMovingManager(ACTION_ACTION);
  ActiveTetromino_t o_after = getCurrentTetrominoInfo();
  ck_assert_int_eq(shapes_equal(o_before.base.shape, o_after.base.shape), 1);
}
END_TEST

START_TEST(tc_no_action_when_checkMerge_true) {
  mock_field_control_reset();

  spawnTetromino((int(*)[4])SHAPE_I);
  ActiveTetromino_t before = getCurrentTetrominoInfo();

  mock_set_checkMerge(1);
  mock_set_canMoveLeft(1);
  mock_set_canMoveRight(1);
  mock_set_canMoveDown(1);
  mock_set_canPlace(1);
  int drops_before = mock_get_drop_called();

  tetrominoMovingManager(ACTION_LEFT);
  tetrominoMovingManager(ACTION_RIGHT);
  tetrominoMovingManager(ACTION_DOWN);
  tetrominoMovingManager(ACTION_ACTION);
  tetrominoMovingManager(ACTION_NONE);

  ActiveTetromino_t after = getCurrentTetrominoInfo();
  ck_assert_int_eq(after.x, before.x);
  ck_assert_int_eq(after.y, before.y);
  ck_assert_int_eq(mock_get_drop_called(), drops_before);
}
END_TEST

// =====
// SUITE

Suite *tetromino_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("tetromino");
  tc_core = tcase_create("core");

  tcase_add_test(tc_core, tc_detect_all_types);
  tcase_add_test(tc_core, tc_spawn_sets_type_and_position);
  tcase_add_test(tc_core, tc_move_left_right_down_and_drop);
  tcase_add_test(tc_core, tc_rotate_non_O_and_O_behaviour);
  tcase_add_test(tc_core, tc_no_action_when_checkMerge_true);

  suite_add_tcase(s, tc_core);

  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite *s = tetromino_suite();
  SRunner *sr = srunner_create(s);

  printf("Running tetromino tests...\n");
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All tetromino tests passed\n\n");
  else
    printf("Tetromino tests failed: %d\n\n", failed);

  return failed;
}