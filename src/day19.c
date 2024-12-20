#include "main.h"

#define DAY19_MAX_TOWEL_LEN   8
#define DAY19_BITS_PER_COLOR  3
#define DAY19_MAX_PATTERN_LEN 60
#define DAY19_HASH_CAPACITY   Kb(16)

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

structdef(Day19Str) {
    char chars[DAY19_MAX_PATTERN_LEN];
};

internal u64 day19_encode_towel(Day19Str* str) {
    u64   ret = 0;
    char* p   = str->chars;
    while (*p) {
        ret <<= DAY19_BITS_PER_COLOR;
        ret  |= day19_encode_color(*p++);
    }
    return ret;
}

internal u64 day19_pattern_possible(bool* towel_exists, HashArray* seen_patterns, Day19Str pattern) {
    usize pattern_count = strlen(pattern.chars);
    if (pattern_count == 0) return 1;

    void* cached = hasharray_maybe_get(seen_patterns, &pattern);
    if (cached) return *(u64*)cached;

    u64 result  = 0;
    u64 max_len = Min(DAY19_MAX_TOWEL_LEN, pattern_count);

    for (u64 len = 1; len <= max_len; ++len) {
        Day19Str chunk = pattern;
        memset(&chunk.chars[len], 0, DAY19_MAX_PATTERN_LEN - len);

        if (towel_exists[day19_encode_towel(&chunk)]) {
            Day19Str rest = {0};
            memcpy(rest.chars, &pattern.chars[len], pattern_count - len);
            result += day19_pattern_possible(towel_exists, seen_patterns, rest);
        }
    }

    *(u64*)hasharray_insert(seen_patterns, &pattern) = result;

    return result;
}

internal DayResult day19(Arena* arena, Str input) {
    HashArray* seen_patterns = hasharray_alloc_with_cap(arena, sizeof(Day19Str), sizeof(u64), DAY19_HASH_CAPACITY);
    bool*      towel_exists  = ArenaAlloc(bool, arena, (1 << (DAY19_MAX_TOWEL_LEN * DAY19_BITS_PER_COLOR)));

    StrSplitIter lines_it = StrSplitIter_new('\n', input);
    foreach (StrSplitIter, towel_it, ',', lines_it.item) {
        Str      trimmed = str_trim(towel_it.item);
        Day19Str rest    = {0};
        memcpy(rest.chars, trimmed.items, trimmed.count);
        u64 towel           = day19_encode_towel(&rest);
        towel_exists[towel] = true;
    }

    StrSplitIter_next(&lines_it);
    StrSplitIter_next(&lines_it);

    i64 part1 = 0;
    i64 part2 = 0;
    for (; !lines_it.done; StrSplitIter_next(&lines_it)) {
        Day19Str rest    = {0};
        Str      trimmed = str_trim(lines_it.item);
        memcpy(rest.chars, trimmed.items, trimmed.count);
        u64 count  = day19_pattern_possible(towel_exists, seen_patterns, rest);
        part1     += count != 0;
        part2     += count;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}