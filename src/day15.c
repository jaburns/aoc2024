#include "main.h"

#if TEST_INPUTS
#define DAY15_SIZE 10
#else
#define DAY15_SIZE 50
#endif

structdef(Day15Grid) {
    char* cells;
    u32   width;
    u32   height;
    u32   stride;
};

internal ivec2 day15_get_start_pos(Day15Grid grid) {
    ivec2 p;
    for (p.y = 0; p.y < grid.height; ++p.y) {
        for (p.x = 0; p.x < grid.width; ++p.x) {
            if (grid.cells[p.x + grid.stride * p.y] == '@') {
                return p;
            }
        }
    }
    AssertUnreachable();
}

internal bool day15_can_move(Day15Grid grid, ivec2 pos, ivec2 dir) {
    ivec2 dest = ivec2_add(pos, dir);

    switch (grid.cells[dest.x + grid.stride * dest.y]) {
        case '[':
            if (dir.y == 0) goto case_wide_horizontal;
            return day15_can_move(grid, dest, dir) && day15_can_move(grid, ivec2_add(dest, IVEC2_RIGHT), dir);
        case ']':
            if (dir.y == 0) goto case_wide_horizontal;
            return day15_can_move(grid, dest, dir) && day15_can_move(grid, ivec2_add(dest, IVEC2_LEFT), dir);
        case 'O':
        case '@':
        case_wide_horizontal:
            return day15_can_move(grid, dest, dir);
        case '.':
            return true;
        case '#':
            return false;
        default:
            AssertUnreachable();
    }
}

internal void day15_exec_move(Day15Grid grid, ivec2 pos, ivec2 dir) {
    ivec2 dest      = ivec2_add(pos, dir);
    char* cur_cell  = &grid.cells[pos.x + grid.stride * pos.y];
    char* dest_cell = &grid.cells[dest.x + grid.stride * dest.y];

    switch (*dest_cell) {
        case '[':
            if (dir.y == 0) goto case_wide_horizontal;
            day15_exec_move(grid, dest, dir);
            day15_exec_move(grid, ivec2_add(dest, IVEC2_RIGHT), dir);
            break;
        case ']':
            if (dir.y == 0) goto case_wide_horizontal;
            day15_exec_move(grid, dest, dir);
            day15_exec_move(grid, ivec2_add(dest, IVEC2_LEFT), dir);
            break;
        case 'O':
        case '@':
        case_wide_horizontal:
            day15_exec_move(grid, dest, dir);
            break;
        case '.':
            break;
        case '#':
            return;
        default:
            AssertUnreachable();
    }

    *dest_cell = *cur_cell;
    *cur_cell  = '.';
}

internal ivec2 day15_step(Day15Grid grid, ivec2 pos, char instruction) {
    ivec2 dir;
    switch (instruction) {
        case '<':
            dir = IVEC2_LEFT;
            break;
        case '>':
            dir = IVEC2_RIGHT;
            break;
        case '^':
            dir = IVEC2_DOWN;
            break;
        case 'v':
            dir = IVEC2_UP;
            break;
        default:
            return pos;
    }
    if (!day15_can_move(grid, pos, dir)) return pos;
    day15_exec_move(grid, pos, dir);
    return ivec2_add(pos, dir);
}

#if TEST_INPUTS && DEBUG
internal void day15_print(Day15Grid grid, ivec2 pos) {
    printf("\n");
    for (u32 y = 0; y < grid.height; ++y) {
        for (u32 x = 0; x < grid.width; ++x) {
            printf("%c", grid.cells[x + grid.stride * y]);
        }
        printf("\n");
    }
}
#else
internal void day15_print(Day15Grid grid, ivec2 pos) {}
#endif

internal u64 day15_solve_grid(Day15Grid grid, Slice_char program) {
    ivec2 pos = day15_get_start_pos(grid);

    for (u32 i = 0; i < program.count; ++i) {
        day15_print(grid, pos);
        pos = day15_step(grid, pos, program.items[i]);
    }
    day15_print(grid, pos);

    u64 result = 0;
    for (u32 y = 0; y < grid.height; ++y) {
        for (u32 x = 0; x < grid.width; ++x) {
            char cell = grid.cells[x + grid.stride * y];
            if (cell == 'O' || cell == '[') {
                result += 100 * y + x;
            }
        }
    }

    return result;
}

#define DAY15_WIDECHAR(a, b) ((u16)(a) | ((u16)(b) << 8))

internal DayResult day15(Arena* arena, Str input) {
    usize input_grid_size = (DAY15_SIZE + 1) * DAY15_SIZE;

    Day15Grid grid1 = (Day15Grid){
        .cells  = ArrayAlloc(char, arena, input_grid_size),
        .width  = DAY15_SIZE,
        .height = DAY15_SIZE,
        .stride = DAY15_SIZE + 1,
    };
    ArrayCopy(grid1.cells, input.items, 0, input_grid_size);

    usize      program_offset = input_grid_size + 1;
    Slice_char program        = {input.items + program_offset, input.count - program_offset};

    Day15Grid grid2 = (Day15Grid){
        .cells  = ArrayAlloc(char, arena, 2 * DAY15_SIZE * DAY15_SIZE),
        .width  = 2 * DAY15_SIZE,
        .height = DAY15_SIZE,
        .stride = 2 * DAY15_SIZE,
    };

    u16* wide_grid = (u16*)grid2.cells;

    for (u32 y = 0; y < DAY15_SIZE; ++y) {
        for (u32 x = 0; x < DAY15_SIZE; ++x) {
            switch (grid1.cells[x + grid1.stride * y]) {
                case '.':
                    wide_grid[x + DAY15_SIZE * y] = DAY15_WIDECHAR('.', '.');
                    break;
                case '#':
                    wide_grid[x + DAY15_SIZE * y] = DAY15_WIDECHAR('#', '#');
                    break;
                case 'O':
                    wide_grid[x + DAY15_SIZE * y] = DAY15_WIDECHAR('[', ']');
                    break;
                case '@':
                    wide_grid[x + DAY15_SIZE * y] = DAY15_WIDECHAR('@', '.');
                    break;
                default:
                    AssertUnreachable();
            }
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = day15_solve_grid(grid1, program);
    result.parts[1].as_i64 = day15_solve_grid(grid2, program);
    return result;
}