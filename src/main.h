#pragma once

#include "../jaburns_c/base/inc.h"

structdef(DayResultPart) {
    bool is_str;
    union {
        u64 as_u64;
        Str as_str;
    };
};

structdef(DayResult) {
    DayResultPart parts[2];
};

typedef DayResult (*DayFn)(void);