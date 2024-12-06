#include "main.h"

#if TEST_INPUTS
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

#define Walk2(dx, dy, marker)                               \
    char* target = &data[(gx + dx) + (gy + dy) * DAY6_ROW]; \
    if (*target == '#') {                                   \
        mode = (mode + 1) % 4;                              \
        continue;                                           \
    }                                                       \
    if (*target == marker) {                                \
        ret = 1;                                            \
        goto done;                                          \
    }                                                       \
    *target  = marker;                                      \
    gx      += dx;                                          \
    gy      += dy;

internal i32 day6_check_loop(Str input, i32 gx, i32 gy, Day6Mode mode) {
    ArenaTemp scratch = scratch_acquire(NULL, 0);
    char*     data    = arena_alloc_nz(scratch.arena, input.count);
    memcpy(data, input.items, input.count);
    i32 ret = 0;

    switch (mode) {
        case DAY6_UP: {
            data[(gx + 0) + (gy - 1) * DAY6_ROW] = '#';
            break;
        }
        case DAY6_DOWN: {
            data[(gx + 0) + (gy + 1) * DAY6_ROW] = '#';
            break;
        }
        case DAY6_LEFT: {
            data[(gx - 1) + (gy + 0) * DAY6_ROW] = '#';
            break;
        }
        case DAY6_RIGHT: {
            data[(gx + 1) + (gy + 0) * DAY6_ROW] = '#';
            break;
        }
    }

    mode = (mode + 1) % 4;

    for (;;) {
        switch (mode) {
            case DAY6_UP: {
                if (gy == 0) goto done;
                Walk2(0, -1, 'u');
                break;
            }
            case DAY6_DOWN: {
                if (gy == DAY6_SIZE - 1) goto done;
                Walk2(0, 1, 'd');
                break;
            }
            case DAY6_LEFT: {
                if (gx == 0) goto done;
                Walk2(-1, 0, 'l');
                break;
            }
            case DAY6_RIGHT: {
                if (gx == DAY6_SIZE - 1) goto done;
                Walk2(1, 0, 'r');
                break;
            }
        }
    }

done: {}
    scratch_release(scratch);
    return ret;
}

internal DayResult day6(Arena* arena, Str input) {
    char* data  = arena_alloc_nz(arena, input.count);
    char* data1 = arena_alloc(arena, input.count);
    memcpy(data, input.items, input.count);

    i64 part1 = 0;
    i64 part2 = 0;

    char* walk = data;

    i32 gx = 0, gy = 0;

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

    part1         = 1;
    Day6Mode mode = DAY6_UP;

    data[gx + gy * DAY6_ROW] = 'u';

    part2 += day6_check_loop(input, gx, gy, mode);

#define Walk(dx, dy, marker, loop_marker)                    \
    char* current = &data[gx + gy * DAY6_ROW];               \
    char* target  = &data[(gx + dx) + (gy + dy) * DAY6_ROW]; \
    if (*target == '#') {                                    \
        mode = (mode + 1) % 4;                               \
        continue;                                            \
    }                                                        \
    if (*target == '.') {                                    \
        *target = marker;                                    \
        part1++;                                             \
        part2 += day6_check_loop(input, gx, gy, mode);       \
    }                                                        \
    gx += dx;                                                \
    gy += dy;

    for (;;) {
        switch (mode) {
            case DAY6_UP: {
                if (gy == 0) goto done;
                Walk(0, -1, 'u', 'r');
                break;
            }
            case DAY6_DOWN: {
                if (gy == DAY6_SIZE - 1) goto done;
                Walk(0, 1, 'd', 'l');
                break;
            }
            case DAY6_LEFT: {
                if (gx == 0) goto done;
                Walk(-1, 0, 'l', 'u');
                break;
            }
            case DAY6_RIGHT: {
                if (gx == DAY6_SIZE - 1) goto done;
                Walk(1, 0, 'r', 'd');
                break;
            }
        }
    }

done: {}

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}