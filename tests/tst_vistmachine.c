#include "unity.h"
#include "unity_fixture.h"
#include "vistmachine.h"

TEST_GROUP(tst_vistmachine);

TEST_GROUP_RUNNER(tst_vistmachine)
{
  RUN_TEST_CASE(tst_vistmachine, VISTMACHINE_SomeFunc);
}

TEST_SETUP(tst_vistmachine)
{
}

TEST_TEAR_DOWN(tst_vistmachine)
{

}

TEST(tst_vistmachine, VISTMACHINE_SomeFunc)
{
    TEST_ASSERT((VISTMACHINE_SomeFunc() == 3));
}