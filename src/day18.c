#include "main.h"

#if TEST_INPUTS
#define DAY18_SIZE        7
#define DAY18_PART1_COUNT 12
#else
#define DAY18_SIZE        71
#define DAY18_PART1_COUNT 1024
#endif

#define DAY18_ENABLE_PART_2 1

#define DAY18_END_COORD (ivec2(DAY18_SIZE - 1, DAY18_SIZE - 1))

structdef(Day18Cell) {
    Day18Cell* from;
    ivec2      coord;
    i32        gscore;
    i32        fscore;
    u16        wall_idx;
    bool       in_open_set;
};

typedef Day18Cell* Day18CellPtr;
DefArrayTypes(Day18CellPtr);

internal i32 day18_fscore(ivec2 coord) {
    return 2 * DAY18_SIZE - 2 - (coord.x) - (coord.y);
}

internal void day18_step(Vec_Day18CellPtr* open_set, Day18Cell* grid, Day18Cell* cur, ivec2 delta, u16 max_wall) {
    ivec2      ncoord   = ivec2_add(cur->coord, delta);
    Day18Cell* neighbor = &grid[ncoord.x + ncoord.y * DAY18_SIZE];
    if (neighbor->wall_idx > max_wall) {
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

internal bool day18_reset_and_solve(Arena* arena, Day18Cell* grid, u16 max_wall) {
    ArenaMark mk  = arena_mark(arena);
    bool      ret = false;

    for (i32 y = 0, i = 0; y < DAY18_SIZE; ++y) {
        for (i32 x = 0; x < DAY18_SIZE; ++x) {
            grid[i] = (Day18Cell){
                .from        = NULL,
                .coord       = ivec2(x, y),
                .gscore      = INT32_MAX,
                .fscore      = INT32_MAX,
                .wall_idx    = grid[i].wall_idx,
                .in_open_set = false,
            };
            i++;
        }
    }

    Vec_Day18CellPtr open_set = VecAlloc(Day18CellPtr, arena, 2048);

    grid[0].gscore      = 0;
    grid[0].fscore      = day18_fscore(IVEC2_ZERO);
    grid[0].in_open_set = true;
    VecMinHeapPush(Day18CellPtr, open_set, ->fscore, &grid[0]);

    while (open_set.count) {
        Day18Cell* cur;
        VecMinHeapPopInto(Day18CellPtr, open_set, ->fscore, cur);
        cur->in_open_set = false;

        if (ivec2_eq(cur->coord, DAY18_END_COORD)) {
            ret = true;
            goto end;
        }

        if (cur->coord.x > 0) day18_step(&open_set, grid, cur, ivec2(-1, 0), max_wall);
        if (cur->coord.x < DAY18_SIZE - 1) day18_step(&open_set, grid, cur, ivec2(1, 0), max_wall);
        if (cur->coord.y > 0) day18_step(&open_set, grid, cur, ivec2(0, -1), max_wall);
        if (cur->coord.y < DAY18_SIZE - 1) day18_step(&open_set, grid, cur, ivec2(0, 1), max_wall);
    }

end:

    arena_restore(arena, mk);
    return ret;
}

internal DayResult day18(Arena* arena, Str input) {
    Vec_ivec2  input_coords = VecAlloc(ivec2, arena, 4096);
    Day18Cell* grid         = ArenaAlloc(Day18Cell, arena, DAY18_SIZE * DAY18_SIZE);

    for (u32 i = 0; i < DAY18_SIZE * DAY18_SIZE; ++i) {
        grid[i].wall_idx = UINT16_MAX;
    }

    u32 line_count = 0;
    foreach (StrSplitIter, lines_it, '\n', input) {
        StrSplitIter parts_it = StrSplitIter_new(',', lines_it.item);
        i32          x        = str_parse_i32(parts_it.item, 10);
        StrSplitIter_next(&parts_it);
        i32 y = str_parse_i32(parts_it.item, 10);

        grid[x + y * DAY18_SIZE].wall_idx = line_count++;
        *VecPush(input_coords)            = ivec2(x, y);
    }

    day18_reset_and_solve(arena, grid, DAY18_PART1_COUNT);

    DayResult result       = {0};
    result.parts[0].as_i64 = grid[DAY18_END_COORD.x + DAY18_END_COORD.y * DAY18_SIZE].gscore;

#if !DAY18_ENABLE_PART_2
    result.parts[1].as_i64 = 0;
#else

    u16 min = DAY18_PART1_COUNT;
    u16 max = line_count;
    for (;;) {
        u16 pivot = ((max - min) / 2) + min;

        if (day18_reset_and_solve(arena, grid, pivot)) {
            min = pivot;
        } else {
            max = pivot;
        }

        if (max == min + 1) {
            break;
        }
    }

    ivec2 part2_coord = input_coords.items[max];

    Str part2 = SliceAlloc(char, arena, 64);
    sprintf(part2.items, "%d,%d", part2_coord.x, part2_coord.y);

    result.parts[1].is_str = true;
    result.parts[1].as_str = part2;

#endif

    return result;
}