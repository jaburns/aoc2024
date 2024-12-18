#include "main.h"

#if TEST_INPUTS
#define DAY18_SIZE        7
#define DAY18_PART1_COUNT 12
#else
#define DAY18_SIZE        71
#define DAY18_PART1_COUNT 1024
#endif

structdef(Day18Cell) {
    bool       corrupt;
    bool       in_open_set;
    ivec2      coord;
    i32        gscore;
    i32        fscore;
    Day18Cell* from;
};

typedef Day18Cell* Day18CellPtr;
DefArrayTypes(Day18CellPtr);

internal i32 day18_fscore(ivec2 coord) {
    return 2 * DAY18_SIZE - 2 - (coord.x) - (coord.y);
}

internal void day18_step(Vec_Day18CellPtr* open_set, Day18Cell* grid, Day18Cell* cur, ivec2 delta) {
    ivec2      ncoord   = ivec2_add(cur->coord, delta);
    Day18Cell* neighbor = &grid[ncoord.x + ncoord.y * DAY18_SIZE];
    if (!neighbor->corrupt) {
        i32 newg = cur->gscore + 1;
        if (newg < neighbor->gscore) {
            neighbor->from        = cur;
            neighbor->gscore      = newg;
            neighbor->fscore      = newg + day18_fscore(neighbor->coord);
            neighbor->in_open_set = true;
            VecMinHeapPush(Day18CellPtr, *open_set, ->fscore, neighbor);
        }
    }
}

internal DayResult day18(Arena* arena, Str input) {
    Vec_ivec2 data = VecAlloc(ivec2, arena, 4096);

    foreach (StrSplitIter, lines_it, '\n', input) {
        StrSplitIter parts_it = StrSplitIter_new(',', lines_it.item);
        i32          x        = str_parse_i32(parts_it.item, 10);
        StrSplitIter_next(&parts_it);
        i32 y          = str_parse_i32(parts_it.item, 10);
        *VecPush(data) = ivec2(x, y);
    }

    Day18Cell* grid = ArenaAlloc(Day18Cell, arena, DAY18_SIZE * DAY18_SIZE);

    for (i32 i = 0; i < DAY18_PART1_COUNT; ++i) {
        grid[data.items[i].x + data.items[i].y * DAY18_SIZE].corrupt = true;
    }
    {
        i32 i = 0;
        for (i32 y = 0; y < DAY18_SIZE; ++y) {
            for (i32 x = 0; x < DAY18_SIZE; ++x) {
                grid[i].gscore = INT32_MAX;
                grid[i].fscore = INT32_MAX;
                grid[i].coord  = ivec2(x, y);
                i++;
            }
        }
    }

    i64              part1     = 0;
    Vec_Day18CellPtr open_set  = VecAlloc(Day18CellPtr, arena, 2048);
    ivec2            end_coord = ivec2(DAY18_SIZE - 1, DAY18_SIZE - 1);

    grid[0].gscore      = 0;
    grid[0].fscore      = day18_fscore(IVEC2_ZERO);
    grid[0].in_open_set = true;
    VecMinHeapPush(Day18CellPtr, open_set, ->fscore, &grid[0]);

    while (open_set.count) {
        Day18Cell* cur;
        VecMinHeapPopInto(Day18CellPtr, open_set, ->fscore, cur);
        cur->in_open_set = false;

        if (ivec2_eq(cur->coord, end_coord)) {
            part1 = cur->gscore;
            break;
        }

        if (cur->coord.x > 0) day18_step(&open_set, grid, cur, ivec2(-1, 0));
        if (cur->coord.x < DAY18_SIZE - 1) day18_step(&open_set, grid, cur, ivec2(1, 0));
        if (cur->coord.y > 0) day18_step(&open_set, grid, cur, ivec2(0, -1));
        if (cur->coord.y < DAY18_SIZE - 1) day18_step(&open_set, grid, cur, ivec2(0, 1));
    }

    Str part2 = SliceAlloc(char, arena, 64);
    // sprintf(part2.items, "%d,%d", part2_coord.x, part2_coord.y);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].is_str = true;
    result.parts[1].as_str = part2;
    return result;
}