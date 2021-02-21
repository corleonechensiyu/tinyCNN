#ifndef UTIL_H_
#define UTIL_H_
#include <assert.h>
#include <stdio.h>
#if defined(_MSC_VER)
#if defined(BUILDING_DLL)
#define PUBLIC __declspec(dllexport)
#elif defined(USING_DLL)
#define PUBLIC __declspec(dllimport)
#else
#define PUBLIC
#endif
#else
#define PUBLIC __attribute__((visibility("default")))
#endif


#define PRINT(format, ...) printf(format, ##__VA_ARGS__)
#define ERROR(format, ...) printf(format, ##__VA_ARGS__)

#define ASSERT(x)                                            \
    {                                                            \
        int res = (x);                                           \
        if (!res) {                                              \
            ERROR("Error for %s, %d\n", __FILE__, __LINE__); \
            assert(res);                                         \
        }                                                        \
    }
#define TMIN(x, y) ((x) < (y) ? (x) : (y))
#define TMAX(x, y) ((x) > (y) ? (x) : (y))

#define UP_DIV(x, y) (((x) + (y) - (1)) / (y))
#define ROUND_UP(x, y) (((x) + (y) - (1)) / (y) * (y))
#define ALIGN_UP4(x) ROUND_UP((x), 4)
#define ALIGN_UP8(x) ROUND_UP((x), 8)


#endif // !UTIL_H_

