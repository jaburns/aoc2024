#include "main.h"

#define DAY4_WIDTH  140
#define DAY4_HEIGHT 140

internal DayResult day4(Arena* arena, Str input) {
    i64 part1 = 0;
    i64 part2 = 0;

#define Get(x, y) input.items[(x) + (y)]

#define Check1(x1, y1, x2, y2, x3, y3)    \
    if (Get(x + (x1), y + (y1)) == 'M' && \
        Get(x + (x2), y + (y2)) == 'A' && \
        Get(x + (x3), y + (y3)) == 'S') { \
        part1++;                          \
    }

#define Check2(ch0, ch1, ch2, ch3)           \
    if (Get(x - 1, y - row_size) == (ch0) && \
        Get(x - 1, y + row_size) == (ch1) && \
        Get(x + 1, y + row_size) == (ch2) && \
        Get(x + 1, y - row_size) == (ch3)) { \
        part2++;                             \
    }

    i64 row_size = DAY4_WIDTH + 1;
    i64 y_max;

    i64 p1 = row_size;
    i64 p2 = 2 * p1;
    i64 p3 = p2 + p1;
    i64 n1 = -p1;
    i64 n2 = -p2;
    i64 n3 = -p3;

    for (i64 y = 0, y_max = 3 * row_size; y < y_max; y += row_size) {
        for (i64 x = 0; x < 3; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, p1, 0, p2, 0, p3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(1, p1, 2, p2, 3, p3);
            }
        }
        for (i64 x = 3; x < DAY4_WIDTH - 3; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, p1, 0, p2, 0, p3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(-1, 0, -2, 0, -3, 0);
                Check1(1, p1, 2, p2, 3, p3);
                Check1(-1, p1, -2, p2, -3, p3);
            }
        }
        for (i64 x = DAY4_WIDTH - 3; x < DAY4_WIDTH; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, p1, 0, p2, 0, p3);
                Check1(-1, 0, -2, 0, -3, 0);
                Check1(-1, p1, -2, p2, -3, p3);
            }
        }
    }

    for (i64 y = 3 * row_size, y_max = (DAY4_HEIGHT - 3) * row_size; y < y_max; y += row_size) {
        for (i64 x = 0; x < 3; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, p1, 0, p2, 0, p3);
                Check1(0, n1, 0, n2, 0, n3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(1, p1, 2, p2, 3, p3);
                Check1(1, n1, 2, n2, 3, n3);
            }
        }
        for (i64 x = 3; x < DAY4_WIDTH - 3; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, p1, 0, p2, 0, p3);
                Check1(0, n1, 0, n2, 0, n3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(-1, 0, -2, 0, -3, 0);
                Check1(1, p1, 2, p2, 3, p3);
                Check1(-1, n1, -2, n2, -3, n3);
                Check1(-1, p1, -2, p2, -3, p3);
                Check1(1, n1, 2, n2, 3, n3);
            } else if (Get(x, y) == 'A') {
                Check2('M', 'M', 'S', 'S') else Check2('M', 'S', 'S', 'M') else Check2('S', 'S', 'M', 'M') else Check2('S', 'M', 'M', 'S')
            }
        }
        for (i64 x = DAY4_WIDTH - 3; x < DAY4_WIDTH; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, p1, 0, p2, 0, p3);
                Check1(0, n1, 0, n2, 0, n3);
                Check1(-1, 0, -2, 0, -3, 0);
                Check1(-1, n1, -2, n2, -3, n3);
                Check1(-1, p1, -2, p2, -3, p3);
            }
        }
    }
    for (i64 y = (DAY4_HEIGHT - 3) * row_size, y_max = DAY4_HEIGHT * row_size; y < y_max; y += row_size) {
        for (i64 x = 0; x < 3; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, n1, 0, n2, 0, n3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(1, n1, 2, n2, 3, n3);
            }
        }
        for (i64 x = 3; x < DAY4_WIDTH - 3; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, n1, 0, n2, 0, n3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(-1, 0, -2, 0, -3, 0);
                Check1(-1, n1, -2, n2, -3, n3);
                Check1(1, n1, 2, n2, 3, n3);
            }
        }
        for (i64 x = DAY4_WIDTH - 3; x < DAY4_WIDTH; ++x) {
            if (Get(x, y) == 'X') {
                Check1(0, n1, 0, n2, 0, n3);
                Check1(-1, 0, -2, 0, -3, 0);
                Check1(-1, n1, -2, n2, -3, n3);
            }
        }
    }

    for (i64 y = row_size, y_max = 3 * row_size; y < y_max; y += row_size) {
        for (i64 x = 3; x < DAY4_WIDTH - 3; ++x) {
            if (Get(x, y) == 'A') {
                Check2('M', 'M', 'S', 'S') else Check2('M', 'S', 'S', 'M') else Check2('S', 'S', 'M', 'M') else Check2('S', 'M', 'M', 'S')
            }
        }
    }
    for (i64 y = (DAY4_HEIGHT - 3) * row_size, y_max = (DAY4_HEIGHT - 1) * row_size; y < y_max; y += row_size) {
        for (i64 x = 3; x < DAY4_WIDTH - 3; ++x) {
            if (Get(x, y) == 'A') {
                Check2('M', 'M', 'S', 'S') else Check2('M', 'S', 'S', 'M') else Check2('S', 'S', 'M', 'M') else Check2('S', 'M', 'M', 'S')
            }
        }
    }
    for (i64 y = row_size, y_max = (DAY4_HEIGHT - 1) * row_size; y < y_max; y += row_size) {
        for (i64 x = 1; x < 3; ++x) {
            if (Get(x, y) == 'A') {
                Check2('M', 'M', 'S', 'S') else Check2('M', 'S', 'S', 'M') else Check2('S', 'S', 'M', 'M') else Check2('S', 'M', 'M', 'S')
            }
        }
        for (i64 x = DAY4_WIDTH - 3; x < DAY4_WIDTH - 1; ++x) {
            if (Get(x, y) == 'A') {
                Check2('M', 'M', 'S', 'S') else Check2('M', 'S', 'S', 'M') else Check2('S', 'S', 'M', 'M') else Check2('S', 'M', 'M', 'S')
            }
        }
    }

#undef Get
#undef Check1
#undef Check2

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}