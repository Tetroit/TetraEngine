// #include  "tetrapc.h"

#define LIB_TEST false
#define ECS_TEST true

#if LIB_TEST
#include "LibConfigTest.h"
#endif
#if ECS_TEST
#include "ECSTest.h"
#endif

using namespace TetraEngine::Tests;

int main () {

#if LIB_TEST
    LibConfigTest::run();
#endif
#if ECS_TEST
    ECSTest::run();
#endif

    return 0;
}


