#define _GNU_SOURCE

#define UNIT_TEST

#include "../src/core/score_file.c"
#include "include/test_helpers.h"

// ======
// Helper
static void cleanup_test_file(void) { unlink(SCORE_FILE); }

// =====
// TESTS

START_TEST(test_write_with_name) {
  cleanup_test_file();

  GameInfo_t game_info = {0};
  game_info.score = 1000;
  game_info.player = "Player1";

  bool result = writeScoreIntoFile(game_info);
  ck_assert_int_eq(result, TRUE);

  FILE* f = fopen(SCORE_FILE, "r");
  ck_assert_ptr_nonnull(f);

  char buffer[100];
  ck_assert_ptr_nonnull(fgets(buffer, sizeof(buffer), f));
  buffer[strcspn(buffer, "\n")] = 0;
  ck_assert_str_eq(buffer, "Player1    1000");

  fclose(f);
}
END_TEST

START_TEST(test_write_without_name) {
  cleanup_test_file();

  GameInfo_t game_info = {0};
  game_info.score = 500;
  game_info.player = NULL;

  bool result = writeScoreIntoFile(game_info);
  ck_assert_int_eq(result, TRUE);

  FILE* f = fopen(SCORE_FILE, "r");
  ck_assert_ptr_nonnull(f);

  char buffer[100];
  ck_assert_ptr_nonnull(fgets(buffer, sizeof(buffer), f));
  buffer[strcspn(buffer, "\n")] = 0;
  ck_assert_str_eq(buffer, "           500");

  fclose(f);
}
END_TEST

START_TEST(test_write_long_name) {
  cleanup_test_file();

  GameInfo_t game_info = {0};
  game_info.score = 750;
  game_info.player = "VeryLongPlayerNameThatExceedsLimit";

  bool result = writeScoreIntoFile(game_info);
  ck_assert_int_eq(result, TRUE);

  FILE* f = fopen(SCORE_FILE, "r");
  ck_assert_ptr_nonnull(f);

  char buffer[100];
  ck_assert_ptr_nonnull(fgets(buffer, sizeof(buffer), f));
  buffer[strcspn(buffer, "\n")] = 0;
  ck_assert_str_eq(buffer, "VeryLongPl 750");

  fclose(f);
}
END_TEST

START_TEST(test_read_empty_file) {
  cleanup_test_file();

  FILE* f = fopen(SCORE_FILE, "w");
  ck_assert_ptr_nonnull(f);
  fclose(f);

  int result = readHighScore();
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_read_multiple) {
  cleanup_test_file();

  FILE* f = fopen(SCORE_FILE, "w");
  ck_assert_ptr_nonnull(f);
  fprintf(f, "Player1     500\n");
  fprintf(f, "Player2    1200\n");
  fprintf(f, "Player3     800\n");
  fprintf(f, "Player4    1500\n");
  fclose(f);

  int result = readHighScore();
  ck_assert_int_eq(result, 1500);
}
END_TEST

START_TEST(test_read_invalid) {
  cleanup_test_file();

  FILE* f = fopen(SCORE_FILE, "w");
  ck_assert_ptr_nonnull(f);
  fprintf(f, "InvalidLine\n");
  fprintf(f, "Short 999999\n");
  fprintf(f, "Player1    abc\n");
  fprintf(f, "Player2    2500\n");
  fprintf(f, "NoSpace1234\n");
  fprintf(f, "Player3    -500\n");
  fprintf(f, "Player4    1000000\n");
  fprintf(f, "Player5    1800\n");
  fclose(f);

  int result = readHighScore();
  ck_assert_int_eq(result, 2500);
}
END_TEST

START_TEST(test_read_max_score) {
  cleanup_test_file();

  FILE* f = fopen(SCORE_FILE, "w");
  ck_assert_ptr_nonnull(f);
  fprintf(f, "Player1    %d\n", MAX_POINTS);
  fprintf(f, "Player2    500000\n");
  fclose(f);

  int result = readHighScore();
  ck_assert_int_eq(result, MAX_POINTS);
}
END_TEST

START_TEST(test_read_no_file) {
  cleanup_test_file();

  int result = readHighScore();
  ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(test_integration) {
  cleanup_test_file();

  GameInfo_t game1 = {0};
  game1.score = 1000;
  game1.player = "Alice";
  ck_assert_int_eq(writeScoreIntoFile(game1), TRUE);

  GameInfo_t game2 = {0};
  game2.score = 2000;
  game2.player = "Bob";
  ck_assert_int_eq(writeScoreIntoFile(game2), TRUE);

  GameInfo_t game3 = {0};
  game3.score = 1500;
  game3.player = "Charlie";
  ck_assert_int_eq(writeScoreIntoFile(game3), TRUE);

  int high_score = readHighScore();
  ck_assert_int_eq(high_score, 2000);
}
END_TEST

START_TEST(test_same_scores) {
  cleanup_test_file();

  GameInfo_t game1 = {0};
  game1.score = 500;
  game1.player = "Player1";
  writeScoreIntoFile(game1);

  GameInfo_t game2 = {0};
  game2.score = 500;
  game2.player = "Player2";
  writeScoreIntoFile(game2);

  GameInfo_t game3 = {0};
  game3.score = 500;
  game3.player = "Player3";
  writeScoreIntoFile(game3);

  int result = readHighScore();
  ck_assert_int_eq(result, 500);
}
END_TEST

// =====
// SUITE

Suite* score_file_suite(void) {
  Suite* s = suite_create("score_file");
  TCase* tc_core = tcase_create("core");

  tcase_add_test(tc_core, test_write_with_name);
  tcase_add_test(tc_core, test_write_without_name);
  tcase_add_test(tc_core, test_write_long_name);
  tcase_add_test(tc_core, test_read_empty_file);
  tcase_add_test(tc_core, test_read_multiple);
  tcase_add_test(tc_core, test_read_invalid);
  tcase_add_test(tc_core, test_read_max_score);
  tcase_add_test(tc_core, test_read_no_file);
  tcase_add_test(tc_core, test_integration);
  tcase_add_test(tc_core, test_same_scores);

  suite_add_tcase(s, tc_core);
  return s;
}

// ====
// MAIN

int main(void) {
  int failed = 0;
  Suite* s = score_file_suite();
  SRunner* sr = srunner_create(s);

  printf("Running score_file tests...\n");
  srunner_run_all(sr, CK_NORMAL);
  failed += srunner_ntests_failed(sr);
  srunner_free(sr);

  if (failed == 0)
    printf("All score_file tests passed\n\n");
  else
    printf("Score_file tests failed: %d\n\n", failed);

  return failed;
}