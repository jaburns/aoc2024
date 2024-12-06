#include "main.h"

#if BIGBOY_INPUTS
#define DAY6_SIZE 32768
#elif TEST_INPUTS
#define DAY6_SIZE 10
#else
#define DAY6_SIZE 130
#endif

#define DAY6_ROW (DAY6_SIZE + 1)

enumdef(Day6Mode, u8){
    DAY6_UP    = 0,
    DAY6_RIGHT = 1,
    DAY6_DOWN  = 2,
    DAY6_LEFT  = 3,
};

structdef(Day6Visited) {
    i32  x;
    i32  y;
    char old_value;
};
DefArrayTypes(Day6Visited);

internal i32 day6_check_loop(Vec_Day6Visited* visited, char* data, i32 gx, i32 gy, Day6Mode mode) {
    i32 ret = 0;
    mode    = (mode + 1) % 4;

#define Walk(dx, dy)                                                                                  \
    {                                                                                                 \
        char* target = &data[gx + dx + (gy + dy) * DAY6_ROW];                                         \
        if (*target == '#') {                                                                         \
            char* current = &data[gx + gy * DAY6_ROW];                                                \
            if (*current == mode) {                                                                   \
                ret = 1;                                                                              \
                goto end;                                                                             \
            }                                                                                         \
            if (*current > 3) {                                                                       \
                *VecPushUnchecked(*visited) = (Day6Visited){.x = gx, .y = gy, .old_value = *current}; \
                *current                    = mode;                                                   \
            }                                                                                         \
            mode = (mode + 1) % 4;                                                                    \
            continue;                                                                                 \
        }                                                                                             \
        gx += dx;                                                                                     \
        gy += dy;                                                                                     \
    }

    for (;;) {
        switch (mode) {
            case DAY6_UP: {
                if (gy == 0) goto end;
                Walk(0, -1);
                break;
            }
            case DAY6_DOWN: {
                if (gy == DAY6_SIZE - 1) goto end;
                Walk(0, 1);
                break;
            }
            case DAY6_LEFT: {
                if (gx == 0) goto end;
                Walk(-1, 0);
                break;
            }
            case DAY6_RIGHT: {
                if (gx == DAY6_SIZE - 1) goto end;
                Walk(1, 0);
                break;
            }
        }
    }

#undef Walk

end:
    for (u32 i = 0; i < visited->count; ++i) {
        i32 x                    = visited->items[i].x;
        i32 y                    = visited->items[i].y;
        data[(x) + y * DAY6_ROW] = visited->items[i].old_value;
    }
    visited->count = 0;

    return ret;
}

internal DayResult day6(Arena* arena, Str input) {
    char*           data    = arena_alloc_nz(arena, input.count);
    Vec_Day6Visited visited = VecAlloc(Day6Visited, arena, Kb(32));
    memcpy(data, input.items, input.count);

    i32 gx = 0, gy = 0;
    {
        char* walk = data;
        for (u32 y = 0; y < DAY6_SIZE; ++y) {
            for (u32 x = 0; x < DAY6_SIZE; ++x) {
                if (*walk++ == '^') {
                    gx = x;
                    gy = y;
                    goto found_start;
                }
            }
            walk++;
        }
    found_start: {}
    }

    i64      part1 = 1;
    Day6Mode mode  = DAY6_UP;

    data[gx + gy * DAY6_ROW] = 'x';

    u32 target0   = gx + (gy - 1) * DAY6_ROW;
    data[target0] = '#';
    i64 part2     = day6_check_loop(&visited, data, gx, gy, mode);
    data[target0] = '.';

#define Walk(dx, dy)                                                  \
    {                                                                 \
        char* target = &data[(gx + dx) + (gy + dy) * DAY6_ROW];       \
        if (*target == '#') {                                         \
            mode = (mode + 1) % 4;                                    \
            continue;                                                 \
        }                                                             \
        if (*target == '.') {                                         \
            *target  = '#';                                           \
            part2   += day6_check_loop(&visited, data, gx, gy, mode); \
            *target  = 'x';                                           \
            part1++;                                                  \
        }                                                             \
        gx += dx;                                                     \
        gy += dy;                                                     \
    }

    for (;;) {
        switch (mode) {
            case DAY6_UP: {
                if (gy == 0) goto done;
                Walk(0, -1);
                break;
            }
            case DAY6_DOWN: {
                if (gy == DAY6_SIZE - 1) goto done;
                Walk(0, 1);
                break;
            }
            case DAY6_LEFT: {
                if (gx == 0) goto done;
                Walk(-1, 0);
                break;
            }
            case DAY6_RIGHT: {
                if (gx == DAY6_SIZE - 1) goto done;
                Walk(1, 0);
                break;
            }
        }
    }

#undef Walk

done: {}
    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}