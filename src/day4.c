#include "main.h"

#define DAY4_WIDTH  140
#define DAY4_HEIGHT 140

internal DayResult day4(Arena* arena, Str input) {
    i32 part1 = 0;
    i32 part2 = 0;

#define Get(x, y) ((x) < 0 || (x) >= DAY4_WIDTH || (y) < 0 || (y) > DAY4_HEIGHT ? ' ' : input.items[(DAY4_WIDTH + 1) * (y) + (x)])
#define Check1(x1, y1, x2, y2, x3, y3)                                                            \
    if (Get(x + x1, y + y1) == 'M' && Get(x + x2, y + y2) == 'A' && Get(x + x3, y + y3) == 'S') { \
        part1++;                                                                                  \
    }
#define Check2(ch0, ch1, ch2, ch3)                                                                                      \
    if (Get(x - 1, y - 1) == ch0 && Get(x - 1, y + 1) == ch1 && Get(x + 1, y + 1) == ch2 && Get(x + 1, y - 1) == ch3) { \
        part2++;                                                                                                        \
        continue;                                                                                                       \
    }

    for (i32 y = 0; y < DAY4_HEIGHT; ++y) {
        for (i32 x = 0; x < DAY4_WIDTH; ++x) {
            char middle = Get(x, y);
            if (middle == 'X') {
                Check1(0, 1, 0, 2, 0, 3);
                Check1(0, -1, 0, -2, 0, -3);
                Check1(1, 0, 2, 0, 3, 0);
                Check1(-1, 0, -2, 0, -3, 0);

                Check1(1, 1, 2, 2, 3, 3);
                Check1(-1, -1, -2, -2, -3, -3);
                Check1(-1, 1, -2, 2, -3, 3);
                Check1(1, -1, 2, -2, 3, -3);
            } else if (middle == 'A') {
                Check2('M', 'M', 'S', 'S');
                Check2('M', 'S', 'S', 'M');
                Check2('S', 'S', 'M', 'M');
                Check2('S', 'M', 'M', 'S');
            }
        }
    }

#undef Get
#undef Check

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}