#include <psxgpu.h>

void test(const char* name, bool test)
{
    const char* testValue = test ? "OK!" : "FAIL!";
    FntPrint(-1, "%s = %s\n", name, testValue);
}