#include "main.h"

#if TEST_INPUTS
#define DAY16_SIZE 15
#else
#define DAY16_SIZE 141
#endif

#define DAY16_STRIDE    (DAY16_SIZE + 1)
#define DAY16_START_POS (ivec2(1, DAY16_SIZE - 2))

enumdef(Day16Dir, u8){
    Day16Dir_UP,
    Day16Dir_DOWN,
    Day16Dir_LEFT,
    Day16Dir_RIGHT,
};

structdef(Day16Branch) {
    ivec2    pos;
    u32      cost;
    Day16Dir dir;
};
DefArrayTypes(Day16Branch);

internal void day16_walk_back(ivec2 pos, Day16Dir dir, u32 cost, u32* right_costs, u32* left_costs, u32* down_costs, u32* up_costs, char* maze) {
    if (dir == Day16Dir_DOWN) pos.y--;
    if (dir == Day16Dir_UP) pos.y++;
    if (dir == Day16Dir_RIGHT) pos.x--;
    if (dir == Day16Dir_LEFT) pos.x++;

    maze[pos.x + pos.y * DAY16_STRIDE] = 'O';

    if (ivec2_eq(pos, DAY16_START_POS)) return;

#define Check(check_dir, check_costs)                                                                        \
    {                                                                                                        \
        u32 test_cost = dir == check_dir ? cost - 1 : cost > 1000 ? cost - 1001                              \
                                                                  : 0xFFFF;                                  \
        u32 read_cost = check_costs[pos.x + pos.y * DAY16_STRIDE];                                           \
        if (read_cost == test_cost) {                                                                        \
            day16_walk_back(pos, check_dir, test_cost, right_costs, left_costs, down_costs, up_costs, maze); \
        }                                                                                                    \
    }

    Check(Day16Dir_UP, up_costs);
    Check(Day16Dir_DOWN, down_costs);
    Check(Day16Dir_LEFT, left_costs);
    Check(Day16Dir_RIGHT, right_costs);

#undef Check
}

internal DayResult day16(Arena* arena, Str input) {
    i64 part1 = 0;
    i64 part2 = 0;

    char*           maze        = str_copy(arena, input).items;
    u32*            right_costs = ArrayAlloc(u32, arena, input.count);
    u32*            left_costs  = ArrayAlloc(u32, arena, input.count);
    u32*            down_costs  = ArrayAlloc(u32, arena, input.count);
    u32*            up_costs    = ArrayAlloc(u32, arena, input.count);
    Vec_Day16Branch branches    = VecAlloc(Day16Branch, arena, 128);

    Day16Branch init_branch = {
        .pos = DAY16_START_POS,
        .dir = Day16Dir_RIGHT
    };
    VecMinHeapPush(Day16Branch, branches, .cost, init_branch);

    DebugAssert(maze[DAY16_START_POS.x + DAY16_START_POS.y * DAY16_STRIDE] == 'S');

    while (branches.count) {
        Day16Branch b;
        VecMinHeapPopInto(Day16Branch, branches, .cost, b);

        u32 idx = b.pos.x + b.pos.y * DAY16_STRIDE;
        if (maze[idx] == 'E') {
            maze[idx] = 'O';
            part1     = b.cost;
            day16_walk_back(b.pos, b.dir, b.cost, right_costs, left_costs, down_costs, up_costs, maze);
            break;
        }

#define Check(from, to, dx, dy, costs)                          \
    if (b.dir != (from)) {                                      \
        idx = b.pos.x + (dx) + (b.pos.y + (dy)) * DAY16_STRIDE; \
        if (maze[idx] != '#' && costs[idx] == 0) {              \
            Day16Branch new  = b;                               \
            new.pos.x       += (dx);                            \
            new.pos.y       += (dy);                            \
            if (new.dir != (to)) {                              \
                new.dir   = (to);                               \
                new.cost += 1000;                               \
            }                                                   \
            new.cost++;                                         \
            costs[idx] = new.cost;                              \
            VecMinHeapPush(Day16Branch, branches, .cost, new);  \
        }                                                       \
    }

        Check(Day16Dir_LEFT, Day16Dir_RIGHT, 1, 0, right_costs);
        Check(Day16Dir_RIGHT, Day16Dir_LEFT, -1, 0, left_costs);
        Check(Day16Dir_UP, Day16Dir_DOWN, 0, 1, down_costs);
        Check(Day16Dir_DOWN, Day16Dir_UP, 0, -1, up_costs);

#undef Check
    }

    for (u32 y = 0; y < DAY16_SIZE; ++y) {
        for (u32 x = 0; x < DAY16_SIZE; ++x) {
            part2 += maze[x + DAY16_STRIDE * y] == 'O';
        }
    }

#if DEBUG && TEST_INPUTS
    for (u32 y = 0; y < DAY16_SIZE; ++y) {
        for (u32 x = 0; x < DAY16_SIZE; ++x) {
            u32 i = x + DAY16_STRIDE * y;
            printf("%04X|%04X|%04X|%04X ", (u16)left_costs[i], (u16)down_costs[i], (u16)up_costs[i], (u16)right_costs[i]);
        }
        printf("\n");
    }
    printf("\n");
#endif

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}