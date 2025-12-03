// TODO: Test user.c
#include <CUnit/Basic.h>

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 */
int init_user_suite(void)
{
    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_user_suite(void)
{
    return 0;
}

/* Simple dummy test.
 */
void test_dummy(void)
{
    CU_ASSERT(0 == 0);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite userSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   userSuite = CU_add_suite("UserSuite", init_user_suite, clean_user_suite);
   if (NULL == userSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* ORDER IS IMPORTANT */
   if (NULL == CU_add_test(userSuite, "dummy test passed", test_dummy))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
