#include <stdlib.h>
#include <check.h>
#include "../src/tritravel.c"

void
setup (void)
{
  
}

void
teardown (void)
{
  
}

START_TEST (test_validateTriCount)
{
  ck_assert_int_eq (validateTriCount(1), 1);
  ck_assert_int_eq (validateTriCount(3), 2);
  ck_assert_int_eq (validateTriCount(6), 3);
  ck_assert_int_eq (validateTriCount(10), 4);
  ck_assert_int_eq (validateTriCount(15), 5);

  ck_assert_int_eq (validateTriCount(0), 0);
  ck_assert_int_eq (validateTriCount(2), 0);
  ck_assert_int_eq (validateTriCount(-1), 0);
  ck_assert_int_eq (validateTriCount(4), 0);
  ck_assert_int_eq (validateTriCount(13), 0);
}
END_TEST

START_TEST (test_tritravel)
{
	int a[]= {1,6,7,4,-1,6,5,8,9,0};
	ck_assert_int_eq( tritravel(a, sizeof(a)/sizeof(int)), 23);
	int b[] = {5, 4, 1, 2, 8, -1};
	ck_assert_int_eq( tritravel(b, sizeof(b)/sizeof(int)), 17);
	int c[] = {1};
	ck_assert_int_eq( tritravel(c, 1), 1);
}
END_TEST

START_TEST (test_money_create_zero)
{
/*
  Money *m = money_create (0, "USD");
  if(money_amount (m) != 0)
  {
    ck_abort_msg("Zero is a valid amount of money");
  }
*/
}
END_TEST

Suite *
tritravel_suite (void)
{

  Suite *s = suite_create ("tritravel");

  TCase *tc_validateTriCount = tcase_create ("validateTriCount");
  tcase_add_test (tc_validateTriCount, test_validateTriCount);
  tcase_add_test (tc_validateTriCount, test_tritravel);
  suite_add_tcase (s, tc_validateTriCount);

/*
  TCase *tc_limits = tcase_create ("Limits");
  tcase_add_test (tc_limits, test_money_create_neg);
  tcase_add_test (tc_limits, test_money_create_zero);
  suite_add_tcase (s, tc_limits);
*/

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = tritravel_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
