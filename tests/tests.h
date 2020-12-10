#include <psxgpu.h>

int test(const char* name, bool test)
{
    if(!test)
    {
        FntPrint(-1, "%s = FAIL!\n", name);
    }
    return test ? 0 : 1;
}