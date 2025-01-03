#pragma once

#include "../jaburns_c/base/inc.h"

structdef(DayResultPart) {
    bool is_str;
    union {
        i64 as_i64;
        Str as_str;
    };
};

structdef(DayResult) {
    DayResultPart parts[2];
    bool          has_manual_component;
};

typedef DayResult (*DayFn)(Arena* arena, Str input);