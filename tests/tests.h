#include <psxgpu.h>
#include <string.h>

constexpr int test(const char* name, bool test)
{
    if(!test)
    {
        FntPrint(-1, "%s = FAIL!\n", name);
    }
    return test ? 0 : 1;
}

template<typename T, typename U>
struct is_same
{
    static constexpr bool value = false;
};

template<typename T>
struct is_same<T, T>
{
    static constexpr bool value = true;
};

class _assert
{
private:
    unsigned int mTestCount = 0;
    unsigned int mErrorCount = 0; 
    char LastTestWithError[256]{'\0'};
public:
    template<typename testType>
    void AreEqual(const testType& l, const testType& r, int line)
    {        
        mTestCount++;
        const bool test = l == r;
        mErrorCount += test ? 0 : 1;
        if(!test)
        {
            memset(&LastTestWithError[0], '\0', 256);
            if constexpr (is_same<long long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %lld == %lld", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<unsigned long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %lu == %lu", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %ld == %ld", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<int, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %d == %d", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<short, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %d == %d", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<float, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %f == %f", __FUNCTION__, line, l, r);
                return;
            }

            sprintf(&LastTestWithError[0], "%s LINE: %d ASSERT FAILED. Unknown type", __FUNCTION__, line);
        }
    }

    template<typename testType>
    void AreEqual(const testType& l, const testType& r, int line, const char* testName)
    {        
        mTestCount++;
        const bool test = l == r;
        mErrorCount += test ? 0 : 1;
        if(!test)
        {
            memset(&LastTestWithError[0], '\0', 256);
            if constexpr (is_same<long long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %ll == %ll", testName, line, l, r);
                return;
            }
            if constexpr (is_same<unsigned long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %lu == %lu", testName, line, l, r);
                return;
            }
            if constexpr (is_same<long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %ld == %ld", testName, line, l, r);
                return;
            }
            if constexpr (is_same<int, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %d == %d", testName, line, l, r);
                return;
            }

            if constexpr (is_same<short, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %d == %d", testName, line, l, r);
                return;
            }

            if constexpr (is_same<float, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %f == %f", testName, line, l, r);
                return;
            }

            sprintf(&LastTestWithError[0], "%s LINE: %d ASSERT FAILED. Unknown type", testName, line);
        }
    }

    template<typename testType>
    void IsLessThan(const testType& l, const testType& r, int line)
    {        
        mTestCount++;
        const bool test = l < r;
        mErrorCount += test ? 0 : 1;
        if(!test)
        {
            memset(&LastTestWithError[0], '\0', 256);
            if constexpr (is_same<unsigned long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %lu < %lu", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<long, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %ld < %ld", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<int, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %d < %d", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<short, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %d < %d", __FUNCTION__, line, l, r);
                return;
            }
            if constexpr (is_same<float, testType>::value)
            {
                sprintf(&LastTestWithError[0], "%s LINE: %d FAIL: %f < %f", __FUNCTION__, line, l, r);
                return;
            }

            sprintf(&LastTestWithError[0], "%s LINE: %d ASSERT FAILED. Unknown type", __FUNCTION__, line);
        }
    }    

    const char* LastError() { return LastTestWithError; }

    void Reset() { mTestCount = mErrorCount = 0; }

    unsigned int ErrorCount() { return mErrorCount; }
    unsigned int TestCount() { return mTestCount; }    
};

_assert Assert;