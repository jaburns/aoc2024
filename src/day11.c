#include "main.h"

#define DAY11_STONE_POOL_SIZE Gb(8)

structdef(Day11Stone) {
    u64         value;
    Day11Stone* next;
};

internal void day11_print_stones(Day11Stone* stone_stack) {
    Day11Stone* stone = stone_stack;
    while (stone) {
        Dbg(stone->value);
        stone = stone->next;
    }
}

internal u64 day11_count_stones(Day11Stone* stone_stack) {
    Day11Stone* stone  = stone_stack;
    u64         result = 0;
    while (stone) {
        result++;
        stone = stone->next;
    }
    return result;
}

internal u32 day11_count_decimal_digits(u64 b) {
    u32 digits = 0;
    while (b > 0) {
        b /= 10;
        digits++;
    }
    return digits;
}

internal void day11_split(u32 digits, u64 value, u64* a, u64* b) {
    u64 prod = 1;
    while (digits > 0) {
        prod *= 10;
        digits--;
    }

    *a = value / prod;
    *b = value % prod;
}

internal void day11_blink(Day11Stone** stone_pool, Day11Stone* stone_pool_start, Day11Stone* stone_stack) {
    Day11Stone* stone = stone_stack;
    while (stone) {
        Day11Stone* next = stone->next;
        if (stone->value == 0) {
            stone->value = 1;
        } else {
            u32 digits = day11_count_decimal_digits(stone->value);

            if (digits & 1) {
                stone->value *= 2024;
            } else {
                u64 a, b;
                day11_split(digits / 2, stone->value, &a, &b);
                stone->value = a;

                Day11Stone* new_stone = (*stone_pool)++;
                if (new_stone >= &stone_pool_start[DAY11_STONE_POOL_SIZE]) {
                    Panic("Off the end of the pool!");
                }
                new_stone->value = b;

                stone->next     = new_stone;
                new_stone->next = next;
            }
        }
        stone = next;
    }
}

internal DayResult day11(Arena* arena, Str input) {
    u64 part1 = 0;
    u64 part2 = 0;

    Day11Stone* stone_pool       = calloc(DAY11_STONE_POOL_SIZE, sizeof(Day11Stone));
    Day11Stone* stone_pool_start = stone_pool;
    Day11Stone* stone_stack      = NULL;

    foreach (StrSplitWhitespaceIter, it, input) {
        Day11Stone* new_stone = stone_pool++;
        new_stone->value      = str_parse_u64(it.item, 10);
        SllStackPush(stone_stack, new_stone);
    }

    for (u32 i = 0; i < 25; ++i) {
        day11_blink(&stone_pool, stone_pool_start, stone_stack);
    }

    part1 = day11_count_stones(stone_stack);

    for (u32 i = 0; i < 50; ++i) {
        Dbg(i);
        day11_blink(&stone_pool, stone_pool_start, stone_stack);
    }

    part2 = day11_count_stones(stone_stack);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}