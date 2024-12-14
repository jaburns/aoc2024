#include "main.h"

#if BIGBOY_INPUTS
#define DAY4_WIDTH  15000
#define DAY4_HEIGHT 15000
#elif TEST_INPUTS
#define DAY4_WIDTH  10
#define DAY4_HEIGHT 10
#else
#define DAY4_WIDTH  140
#define DAY4_HEIGHT 140
#endif

#define ROW_SIZE (DAY4_WIDTH + 1)
#define XY(x, y) ((x) + (y) * ROW_SIZE)

internal void day4_check1(i64* count, char* input, i64 offset, i64 o1, i64 o2, i64 o3) {
    if (input[offset + o1] == 'M' &&
        input[offset + o2] == 'A' &&
        input[offset + o3] == 'S') {
        ++*count;
    }
}

internal DayResult day4(Arena* arena, Str input) {
    i64 part1 = 0;
    i64 part2 = 0;

    for (i64 y = 0, y_max = DAY4_HEIGHT * ROW_SIZE; y < y_max; y += ROW_SIZE) {
        bool top     = y >= 3 * ROW_SIZE;
        bool bottom  = y < (DAY4_HEIGHT - 3) * ROW_SIZE;
        bool part2_y = y >= 1 * ROW_SIZE && y < (DAY4_HEIGHT - 1) * ROW_SIZE;

        for (i64 x = 0; x < DAY4_WIDTH; ++x) {
            i64 o = x + y;

            char middle = input.items[o];
            if (middle != 'X') {
                if (part2_y &&
                    x >= 1 &&
                    x < DAY4_WIDTH - 1 &&
                    middle == 'A' &&
                    input.items[o + XY(-1, -1)] + input.items[o + XY(1, 1)] == ((i64)('M' + 'S')) &&
                    input.items[o + XY(1, -1)] + input.items[o + XY(-1, 1)] == ((i64)('M' + 'S'))) {
                    ++part2;
                }

                continue;
            }

            bool left  = x >= 3;
            bool right = x < 3 * ROW_SIZE;

            if (bottom) {
                day4_check1(&part1, input.items, o, XY(0, 1), XY(0, 2), XY(0, 3));
                if (right) day4_check1(&part1, input.items, o, XY(1, 1), XY(2, 2), XY(3, 3));
                if (left) day4_check1(&part1, input.items, o, XY(-1, 1), XY(-2, 2), XY(-3, 3));
            }
            if (top) {
                day4_check1(&part1, input.items, o, XY(0, -1), XY(0, -2), XY(0, -3));
                if (right) day4_check1(&part1, input.items, o, XY(1, -1), XY(2, -2), XY(3, -3));
                if (left) day4_check1(&part1, input.items, o, XY(-1, -1), XY(-2, -2), XY(-3, -3));
            }
            if (right) day4_check1(&part1, input.items, o, XY(1, 0), XY(2, 0), XY(3, 0));
            if (left) day4_check1(&part1, input.items, o, XY(-1, 0), XY(-2, 0), XY(-3, 0));
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}

#undef ROW_SIZE
#undef XY