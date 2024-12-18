#include "main.h"

#if TEST_INPUTS
#define DAY18_SIZE        7
#define DAY18_PART1_COUNT 12
#else
#define DAY18_SIZE        71
#define DAY18_PART1_COUNT 1024
#endif

structdef(Day18Cell) {
    bool corrupt;
    u32  steps;
};

internal void day18_walk(Day18Cell* grid, u32 x, u32 y, u32 steps) {
    if (x > 0) {
        u32        x1   = x - 1;
        u32        y1   = y;
        Day18Cell* cell = &grid[x1 + y1 * DAY18_SIZE];
        if (!cell->corrupt && (cell->steps == 0 || cell->steps > steps)) {
            cell->steps = steps;
            day18_walk(grid, x1, y1, steps + 1);
        }
    }

    if (x < DAY18_SIZE - 1) {
        u32        x1   = x + 1;
        u32        y1   = y;
        Day18Cell* cell = &grid[x1 + y1 * DAY18_SIZE];
        if (!cell->corrupt && (cell->steps == 0 || cell->steps > steps)) {
            cell->steps = steps;
            day18_walk(grid, x1, y1, steps + 1);
        }
    }

    if (y > 0) {
        u32        x1   = x;
        u32        y1   = y - 1;
        Day18Cell* cell = &grid[x1 + y1 * DAY18_SIZE];
        if (!cell->corrupt && (cell->steps == 0 || cell->steps > steps)) {
            cell->steps = steps;
            day18_walk(grid, x1, y1, steps + 1);
        }
    }

    if (y < DAY18_SIZE - 1) {
        u32        x1   = x;
        u32        y1   = y + 1;
        Day18Cell* cell = &grid[x1 + y1 * DAY18_SIZE];
        if (!cell->corrupt && (cell->steps == 0 || cell->steps > steps)) {
            cell->steps = steps;
            day18_walk(grid, x1, y1, steps + 1);
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

    i64 part1;
    {
        for (u32 i = 0; i < DAY18_PART1_COUNT; ++i) {
            grid[data.items[i].x + data.items[i].y * DAY18_SIZE].corrupt = true;
        }
        grid[0].steps = 1;
        day18_walk(grid, 0, 0, 2);
        part1 = grid[DAY18_SIZE - 1 + (DAY18_SIZE - 1) * DAY18_SIZE].steps - 1;
    }

    ivec2 part2_coord;
    for (u32 c = DAY18_PART1_COUNT;; ++c) {
        ZeroArray(grid, DAY18_SIZE * DAY18_SIZE);

        for (u32 i = 0; i < c; ++i) {
            grid[data.items[i].x + data.items[i].y * DAY18_SIZE].corrupt = true;
        }

        grid[0].steps = 1;
        day18_walk(grid, 0, 0, 2);
        if (grid[DAY18_SIZE - 1 + (DAY18_SIZE - 1) * DAY18_SIZE].steps == 0) {
            part2_coord = data.items[c - 1];
            break;
        }
    }

    Str part2 = SliceAlloc(char, arena, 64);
    sprintf(part2.items, "%d,%d", part2_coord.x, part2_coord.y);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].is_str = true;
    result.parts[1].as_str = part2;
    return result;
}