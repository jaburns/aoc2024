#include "main.h"

internal u64 day22_step(u64 num) {
    num ^= num << 6;
    num &= 0xFFFFFF;
    num ^= num >> 5;
    num &= 0xFFFFFF;
    num ^= num << 11;
    num &= 0xFFFFFF;
    return num;
}

internal DayResult day22(Arena* arena, Str input) {
    u16* bananas    = ArrayAlloc(u16, arena, 19 * 19 * 19 * 19);
    u16* hits       = ArrayAlloc(u16, arena, 19 * 19 * 19 * 19);
    u32  history[3] = {0};
    u64  part1      = 0;
    u16  part2      = 0;
    u16  line       = 0;

    foreach (StrSplitIter, it, '\n', input) {
        u64 num        = str_parse_u64(it.item, 10);
        u32 prev_price = num % 10;
        line++;

        for (u32 i = 0; i < 3; ++i) {
            num        = day22_step(num);
            u32 price  = num % 10;
            history[i] = 9 + price - prev_price;
            prev_price = price;
        }
        for (u32 i = 3; i < 2000; ++i) {
            num       = day22_step(num);
            u32 price = num % 10;
            u32 delta = 9 + price - prev_price;
            u32 idx   = history[0] + 19 * history[1] + 19 * 19 * history[2] + 19 * 19 * 19 * delta;

            history[0] = history[1];
            history[1] = history[2];
            history[2] = delta;

            if (hits[idx] != line) {
                hits[idx]     = line;
                bananas[idx] += price;
            }

            prev_price = price;
        }

        part1 += num;
    }

    for (u32 i = 0; i < 19 * 19 * 19 * 19; ++i) {
        if (bananas[i] > part2) part2 = bananas[i];
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}