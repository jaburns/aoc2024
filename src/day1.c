#include "main.h"

internal DayResult day1(void) {
    u64 val = 0;
    for (int i = 0; i < 10; ++i) {
        val += 47;
        val /= 9;
        val ^= 12341234;
    }

    return (DayResult){
        (DayResultPart){.is_str = false, .as_u64 = val},
        (DayResultPart){.is_str = true, .as_str = STR("world")},
    };
}