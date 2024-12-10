#include "main.h"

#if TEST_INPUTS
#define DAY10_SIZE 8
#else
#define DAY10_SIZE 46
#endif

#define DAY10_STRIDE (DAY10_SIZE + 1)

internal void day10_walk(char* input, bool* flagged, i32 x, i32 y, char seeking, u64* part1, u64* part2) {
    bool done = seeking == '9';

    if (x > 0 && input[x - 1 + y * DAY10_STRIDE] == seeking) {
        if (done) {
            if (!flagged[x - 1 + y * DAY10_STRIDE]) {
                flagged[x - 1 + y * DAY10_STRIDE] = true;
                ++*part1;
            }
            ++*part2;
        } else {
            day10_walk(input, flagged, x - 1, y, seeking + 1, part1, part2);
        }
    }
    if (x < DAY10_STRIDE - 1 && input[x + 1 + y * DAY10_STRIDE] == seeking) {
        if (done) {
            if (!flagged[x + 1 + y * DAY10_STRIDE]) {
                flagged[x + 1 + y * DAY10_STRIDE] = true;
                ++*part1;
            }
            ++*part2;
        } else {
            day10_walk(input, flagged, x + 1, y, seeking + 1, part1, part2);
        }
    }
    if (y > 0 && input[x + (y - 1) * DAY10_STRIDE] == seeking) {
        if (done) {
            if (!flagged[x + (y - 1) * DAY10_STRIDE]) {
                flagged[x + (y - 1) * DAY10_STRIDE] = true;
                ++*part1;
            }
            ++*part2;
        } else {
            day10_walk(input, flagged, x, y - 1, seeking + 1, part1, part2);
        }
    }
    if (y < DAY10_STRIDE - 1 && input[x + (y + 1) * DAY10_STRIDE] == seeking) {
        if (done) {
            if (!flagged[x + (y + 1) * DAY10_STRIDE]) {
                flagged[x + (y + 1) * DAY10_STRIDE] = true;
                ++*part1;
            }
            ++*part2;
        } else {
            day10_walk(input, flagged, x, y + 1, seeking + 1, part1, part2);
        }
    }
}

internal DayResult day10(Arena* arena, Str input) {
    u64 part1 = 0;
    u64 part2 = 0;

    i32 y = 0;
    foreach (StrSplitIter, lines_it, '\n', input) {
        for (i32 x = 0; x < DAY10_SIZE; ++x) {
            char ch = lines_it.item.items[x];
            if (ch == '0') {
                ArenaMark mk      = arena_mark(arena);
                bool*     flagged = ArenaAlloc(bool, arena, input.count);
                day10_walk(input.items, flagged, x, y, '1', &part1, &part2);
                arena_restore(arena, mk);
            }
        }
        y++;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}