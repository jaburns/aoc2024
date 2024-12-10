#include "main.h"

#if TEST_INPUTS
#define DAY10_SIZE 8
#else
#define DAY10_SIZE 46
#endif

#define DAY10_STRIDE (DAY10_SIZE + 1)

internal void day10_walk(char* input, bool* flagged, i32 x, i32 y, char seeking, u64* part1, u64* part2) {
    u32 left  = x - 1 + y * DAY10_STRIDE;
    u32 right = x + 1 + y * DAY10_STRIDE;
    u32 up    = x + (y - 1) * DAY10_STRIDE;
    u32 down  = x + (y + 1) * DAY10_STRIDE;

    if (seeking == '9') {
        if (x > 0 && input[left] == seeking) {
            *part1        += !flagged[left];
            flagged[left]  = true;
            ++*part2;
        }
        if (x < DAY10_SIZE - 1 && input[right] == seeking) {
            *part1         += !flagged[right];
            flagged[right]  = true;
            ++*part2;
        }
        if (y > 0 && input[up] == seeking) {
            *part1      += !flagged[up];
            flagged[up]  = true;
            ++*part2;
        }
        if (y < DAY10_SIZE - 1 && input[down] == seeking) {
            *part1        += !flagged[down];
            flagged[down]  = true;
            ++*part2;
        }
    } else {
        if (x > 0 && input[left] == seeking) {
            day10_walk(input, flagged, x - 1, y, seeking + 1, part1, part2);
        }
        if (x < DAY10_SIZE - 1 && input[right] == seeking) {
            day10_walk(input, flagged, x + 1, y, seeking + 1, part1, part2);
        }
        if (y > 0 && input[up] == seeking) {
            day10_walk(input, flagged, x, y - 1, seeking + 1, part1, part2);
        }
        if (y < DAY10_SIZE - 1 && input[down] == seeking) {
            day10_walk(input, flagged, x, y + 1, seeking + 1, part1, part2);
        }
    }
}

internal DayResult day10(Arena* arena, Str input) {
    bool* flagged = ArenaAlloc(bool, arena, input.count);

    u64 part1 = 0;
    u64 part2 = 0;

    i32 y = 0;
    foreach (StrSplitIter, lines_it, '\n', input) {
        for (i32 x = 0; x < DAY10_SIZE; ++x) {
            char ch = lines_it.item.items[x];
            if (ch == '0') {
                ZeroArray(flagged, input.count);
                day10_walk(input.items, flagged, x, y, '1', &part1, &part2);
            }
        }
        y++;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}