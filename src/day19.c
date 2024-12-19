#include "main.h"

#define DAY19_MAX_TOWEL_LEN  8
#define DAY19_BITS_PER_COLOR 3

internal u8 day19_encode_color(char color) {
    switch (color) {
        case 'w':
            return 0b001;
        case 'u':
            return 0b010;
        case 'b':
            return 0b011;
        case 'r':
            return 0b100;
        case 'g':
            return 0b101;
    }
    AssertUnreachable();
}

internal u32 day19_encode_towel(Str str) {
    u32 ret = 0;
    for (u32 i = 0; i < str.count; ++i) {
        ret <<= DAY19_BITS_PER_COLOR;
        ret  |= day19_encode_color(str.items[i]);
    }
    return ret;
}

internal u32 day19_pattern_possible(bool* towel_exists, Str pattern) {
    if (pattern.count == 0) return 1;

    u32 result = 0;

    u32 max_len = Min(DAY19_MAX_TOWEL_LEN, pattern.count);

    for (u32 len = 1; len <= max_len; ++len) {
        Str chunk   = pattern;
        chunk.count = len;

        if (towel_exists[day19_encode_towel(chunk)]) {
            result += day19_pattern_possible(towel_exists, str_substr_from(pattern, len));
            if (result) return result;
        }
    }

    return result;
}

internal DayResult day19(Arena* arena, Str input) {
    bool* towel_exists = ArenaAlloc(bool, arena, (1 << (DAY19_MAX_TOWEL_LEN * DAY19_BITS_PER_COLOR)));

    StrSplitIter lines_it = StrSplitIter_new('\n', input);
    foreach (StrSplitIter, towel_it, ',', lines_it.item) {
        u32 towel           = day19_encode_towel(str_trim(towel_it.item));
        towel_exists[towel] = true;
    }

    StrSplitIter_next(&lines_it);
    StrSplitIter_next(&lines_it);

    i64 part1 = 0;
    u64 line  = 0;
    for (; !lines_it.done; StrSplitIter_next(&lines_it)) {
        part1 += day19_pattern_possible(towel_exists, lines_it.item);
        line++;
        Dbg(line);
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    return result;
}
