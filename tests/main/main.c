#include "unity_fixture.h"
#include "vistmachine.h"

static void runAllTests(void)
{
    RUN_TEST_GROUP(tst_vistmachine);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, runAllTests);
}