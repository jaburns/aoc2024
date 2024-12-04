#include "main.h"

#define DAY4_WIDTH  140
#define DAY4_HEIGHT 140

#define ROW_SIZE (DAY4_WIDTH + 1)
#define XP1      1
#define XP2      2
#define XP3      3
#define XM1      (-1)
#define XM2      (-2)
#define XM3      (-3)
#define YP1      (1 * ROW_SIZE)
#define YP2      (2 * ROW_SIZE)
#define YP3      (3 * ROW_SIZE)
#define YM1      (-YP1)
#define YM2      (-YP2)
#define YM3      (-YP3)

internal void day4_check1(i64* count, char* input, i64 offset, i64 o1, i64 o2, i64 o3) {
    if (input[offset + o1] == 'M' &&
        input[offset + o2] == 'A' &&
        input[offset + o3] == 'S') {
        ++*count;
    }
}

internal void day4_check2(i64* count, char* input, i64 offset) {
    if (
        input[offset] == 'A' &&
        input[offset - 1 - ROW_SIZE] + input[offset + 1 + ROW_SIZE] == ((i64)('M' + 'S')) &&
        input[offset + 1 - ROW_SIZE] + input[offset - 1 + ROW_SIZE] == ((i64)('M' + 'S'))
    ) {
        ++*count;
    }
}

internal DayResult day4(Arena* arena, Str input) {
    i64 part1 = 0;
    i64 part2 = 0;

    for (i64 y = 0, y_max = 3 * ROW_SIZE; y < y_max; y += ROW_SIZE) {
        for (i64 o = y, o_max = y + 3; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YP1, YP2, YP3);
            day4_check1(&part1, input.items, o, XP1, XP2, XP3);
            day4_check1(&part1, input.items, o, XP1 + YP1, XP2 + YP2, XP3 + YP3);
        }
        for (i64 o = y + 3, o_max = y + DAY4_WIDTH - 3; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YP1, YP2, YP3);
            day4_check1(&part1, input.items, o, XP1, XP2, XP3);
            day4_check1(&part1, input.items, o, XM1, XM2, XM3);
            day4_check1(&part1, input.items, o, XP1 + YP1, XP2 + YP2, XP3 + YP3);
            day4_check1(&part1, input.items, o, XM1 + YP1, XM2 + YP2, XM3 + YP3);
        }
        for (i64 o = y + DAY4_WIDTH - 3, o_max = y + DAY4_WIDTH; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YP1, YP2, YP3);
            day4_check1(&part1, input.items, o, XM1, XM2, XM3);
            day4_check1(&part1, input.items, o, XM1 + YP1, XM2 + YP2, XM3 + YP3);
        }
    }

    for (i64 y = ROW_SIZE, y_max = 3 * ROW_SIZE; y < y_max; y += ROW_SIZE) {
        for (i64 o = y + 1, o_max = y + DAY4_WIDTH - 1; o < o_max; ++o) {
            day4_check2(&part2, input.items, o);
        }
    }

    for (i64 y = 3 * ROW_SIZE, y_max = (DAY4_HEIGHT - 3) * ROW_SIZE; y < y_max; y += ROW_SIZE) {
        for (i64 o = y, o_max = y + 3; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YP1, YP2, YP3);
            day4_check1(&part1, input.items, o, YM1, YM2, YM3);
            day4_check1(&part1, input.items, o, XP1, XP2, XP3);
            day4_check1(&part1, input.items, o, XP1 + YP1, XP2 + YP2, XP3 + YP3);
            day4_check1(&part1, input.items, o, XP1 + YM1, XP2 + YM2, XP3 + YM3);
        }
        for (i64 o = y + 3, o_max = y + DAY4_WIDTH - 3; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YP1, YP2, YP3);
            day4_check1(&part1, input.items, o, YM1, YM2, YM3);
            day4_check1(&part1, input.items, o, XP1, XP2, XP3);
            day4_check1(&part1, input.items, o, XM1, XM2, XM3);
            day4_check1(&part1, input.items, o, XP1 + YP1, XP2 + YP2, XP3 + YP3);
            day4_check1(&part1, input.items, o, XP1 + YM1, XP2 + YM2, XP3 + YM3);
            day4_check1(&part1, input.items, o, XM1 + YP1, XM2 + YP2, XM3 + YP3);
            day4_check1(&part1, input.items, o, XM1 + YM1, XM2 + YM2, XM3 + YM3);
        }
        for (i64 o = y + DAY4_WIDTH - 3, o_max = y + DAY4_WIDTH; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YP1, YP2, YP3);
            day4_check1(&part1, input.items, o, YM1, YM2, YM3);
            day4_check1(&part1, input.items, o, XM1, XM2, XM3);
            day4_check1(&part1, input.items, o, XM1 + YP1, XM2 + YP2, XM3 + YP3);
            day4_check1(&part1, input.items, o, XM1 + YM1, XM2 + YM2, XM3 + YM3);
        }
        for (i64 o = y + 1, o_max = y + DAY4_WIDTH - 1; o < o_max; ++o) {
            day4_check2(&part2, input.items, o);
        }
    }

    for (i64 y = (DAY4_HEIGHT - 3) * ROW_SIZE, y_max = (DAY4_HEIGHT - 1) * ROW_SIZE; y < y_max; y += ROW_SIZE) {
        for (i64 o = y + 1, o_max = y + DAY4_WIDTH - 1; o < o_max; ++o) {
            day4_check2(&part2, input.items, o);
        }
    }

    for (i64 y = (DAY4_HEIGHT - 3) * ROW_SIZE, y_max = DAY4_HEIGHT * ROW_SIZE; y < y_max; y += ROW_SIZE) {
        for (i64 o = y, o_max = y + 3; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YM1, YM2, YM3);
            day4_check1(&part1, input.items, o, XP1, XP2, XP3);
            day4_check1(&part1, input.items, o, XP1 + YM1, XP2 + YM2, XP3 + YM3);
        }
        for (i64 o = y + 3, o_max = y + DAY4_WIDTH - 3; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YM1, YM2, YM3);
            day4_check1(&part1, input.items, o, XP1, XP2, XP3);
            day4_check1(&part1, input.items, o, XM1, XM2, XM3);
            day4_check1(&part1, input.items, o, XP1 + YM1, XP2 + YM2, XP3 + YM3);
            day4_check1(&part1, input.items, o, XM1 + YM1, XM2 + YM2, XM3 + YM3);
        }
        for (i64 o = y + DAY4_WIDTH - 3, o_max = y + DAY4_WIDTH; o < o_max; ++o) {
            if (input.items[o] != 'X') continue;
            day4_check1(&part1, input.items, o, YM1, YM2, YM3);
            day4_check1(&part1, input.items, o, XM1, XM2, XM3);
            day4_check1(&part1, input.items, o, XM1 + YM1, XM2 + YM2, XM3 + YM3);
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}

#undef ROW_SIZE
#undef XP1
#undef XP2
#undef XP3
#undef XM1
#undef XM2
#undef XM3
#undef YP1
#undef YP2
#undef YP3
#undef YM1
#undef YM2
#undef YM3