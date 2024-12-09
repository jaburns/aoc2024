#include "main.h"

internal u64 day9_part1(u32* start, u32* end) {
    u64  result = 0;
    u32* walk   = start;

    for (;;) {
        while (*walk) walk++;
        while (!*end) end--;

        if (walk >= end) break;

        *walk = *end;
        *end  = 0;
    }

    u64 i = 0;
    walk  = start;
    do {
        result += (i++) * (*walk - 1);
    } while (*++walk);

    return result;
}

internal u64 day9_part2(u32* start, u32* end) {
    return 0;
}

internal DayResult day9(Arena* arena, Str input) {
    u32* data  = ArenaAlloc(u32, arena, 10 * input.count);
    u32* data2 = ArenaAlloc(u32, arena, 10 * input.count);

    u32* end     = data;
    u32  file_id = 1;
    for (u32 i = 0;;) {
        u32 value = (i32)(input.items[i] - '0');
        for (u32 j = 0; j < value; ++j) {
            *end++ = file_id;
        }
        file_id++;

        if (++i >= input.count) break;

        value  = (i32)(input.items[i] - '0');
        end   += value;

        if (++i >= input.count) break;
    }

    // ArrayCopy(data2, data, 0, 10 * input.count);

    u64 part1 = day9_part1(data, end);
    u64 part2 = day9_part2(data2, end);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}
