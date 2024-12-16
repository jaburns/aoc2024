#include "main.h"

#if TEST_INPUTS
#define DAY16_SIZE 17
#else
#define DAY16_SIZE 141
#endif

#define DAY16_STRIDE    (DAY16_SIZE + 1)
#define DAY16_START_POS (ivec2(1, DAY16_SIZE - 2))

enumdef(Day16Dir, u8){
    Day16Dir_UP    = 1,
    Day16Dir_DOWN  = 2,
    Day16Dir_LEFT  = 4,
    Day16Dir_RIGHT = 8,
};

structdef(Day16Branch) {
    ivec2    pos;
    u32      cost;
    Day16Dir dir;
};
DefArrayTypes(Day16Branch);

internal DayResult day16(Arena* arena, Str input) {
    i64 part1 = 0;
    i64 part2 = 0;

    char*           maze     = input.items;
    Day16Dir*       flags    = ArenaAlloc(Day16Dir, arena, input.count);
    Vec_Day16Branch branches = VecAlloc(Day16Branch, arena, 128);

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
            if (part1 == 0) {
                part1 = b.cost;
            }
            break;
        }

#define Check(from, to, dx, dy)                                 \
    if (b.dir != (from)) {                                      \
        idx = b.pos.x + (dx) + (b.pos.y + (dy)) * DAY16_STRIDE; \
        if (maze[idx] != '#' && !(flags[idx] & (to))) {         \
            flags[idx]      |= (to);                            \
            Day16Branch new  = b;                               \
            new.pos.x       += (dx);                            \
            new.pos.y       += (dy);                            \
            if (new.dir != (to)) {                              \
                new.dir   = (to);                               \
                new.cost += 1000;                               \
            }                                                   \
            new.cost++;                                         \
            VecMinHeapPush(Day16Branch, branches, .cost, new);  \
        }                                                       \
    }

        Check(Day16Dir_LEFT, Day16Dir_RIGHT, 1, 0);
        Check(Day16Dir_RIGHT, Day16Dir_LEFT, -1, 0);
        Check(Day16Dir_UP, Day16Dir_DOWN, 0, 1);
        Check(Day16Dir_DOWN, Day16Dir_UP, 0, -1);

#undef Check
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}