#include "main.h"

#if TEST_INPUTS
#define DAY12_SIZE 10
#else
#define DAY12_SIZE 140
#endif

#define DAY12_STRIDE (DAY12_SIZE + 1)

structdef(Day12Region) {
    u32 perimeter;
    u32 corners;
    u32 area;
};
DefArrayTypes(Day12Region);

internal void day12_walk(char* input, bool* flagged, i32 x, i32 y, char seeking, Day12Region* region) {
    u32 this = x + y * DAY12_STRIDE;
    if (flagged[this]) return;
    flagged[this] = true;

    region->area++;

    // clang-format off
    bool left      = x == 0              ? false : input[x - 1 +  y      * DAY12_STRIDE] == seeking;
    bool right     = x == DAY12_SIZE - 1 ? false : input[x + 1 +  y      * DAY12_STRIDE] == seeking;
    bool up        = y == 0              ? false : input[x     + (y - 1) * DAY12_STRIDE] == seeking;
    bool down      = y == DAY12_SIZE - 1 ? false : input[x     + (y + 1) * DAY12_STRIDE] == seeking;

    bool upleft    = x == 0              || y == 0              ? false : input[x - 1 + (y - 1) * DAY12_STRIDE] == seeking;
    bool upright   = x == DAY12_SIZE - 1 || y == 0              ? false : input[x + 1 + (y - 1) * DAY12_STRIDE] == seeking;
    bool downleft  = x == 0              || y == DAY12_SIZE - 1 ? false : input[x - 1 + (y + 1) * DAY12_STRIDE] == seeking;
    bool downright = x == DAY12_SIZE - 1 || y == DAY12_SIZE - 1 ? false : input[x + 1 + (y + 1) * DAY12_STRIDE] == seeking;
    // clang-format on

    region->corners += up && left && !upleft;
    region->corners += up && right && !upright;
    region->corners += down && left && !downleft;
    region->corners += down && right && !downright;

    region->corners += !up && !left;
    region->corners += !up && !right;
    region->corners += !down && !left;
    region->corners += !down && !right;

    if (left) {
        day12_walk(input, flagged, x - 1, y, seeking, region);
    } else {
        region->perimeter++;
    }
    if (right) {
        day12_walk(input, flagged, x + 1, y, seeking, region);
    } else {
        region->perimeter++;
    }
    if (up) {
        day12_walk(input, flagged, x, y - 1, seeking, region);
    } else {
        region->perimeter++;
    }
    if (down) {
        day12_walk(input, flagged, x, y + 1, seeking, region);
    } else {
        region->perimeter++;
    }
}

internal DayResult day12(Arena* arena, Str input) {
    u64 part1 = 0;
    u64 part2 = 0;

    bool* flagged = ArenaAlloc(bool, arena, input.count);

    i32 y = 0;
    foreach (StrSplitIter, lines_it, '\n', input) {
        for (i32 x = 0; x < DAY12_SIZE; ++x) {
            if (!flagged[x + y * DAY12_STRIDE]) {
                Day12Region region = {0};
                day12_walk(input.items, flagged, x, y, lines_it.item.items[x], &region);
                part1 += region.area * region.perimeter;
                part2 += region.area * region.corners;
            }
        }
        y++;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}