#include "main.h"

#if TEST_INPUTS
#define DAY14_WIDTH  11
#define DAY14_HEIGHT 7
#else
#define DAY14_WIDTH  101
#define DAY14_HEIGHT 103
#endif

#define DAY14_STRIDE            (DAY14_WIDTH + 1)
#define DAY14_ENABLE_PRINT_MODE 0

structdef(Day14Robot) {
    i64 x, y;
    i64 vx, vy;
};
DefArrayTypes(Day14Robot);

internal void day14_print_pictures(Arena* arena, Slice_Day14Robot robots) {
    usize grid_len = DAY14_STRIDE * DAY14_HEIGHT + 1;
    char* grid     = ArenaAlloc(char, arena, grid_len);

    u64 elapsed = 0;
    for (u32 xx = 0; xx < 7000; xx++) {
        for (u32 y = 0; y < DAY14_HEIGHT; ++y) {
            for (u32 x = 0; x < DAY14_WIDTH; ++x) {
                grid[x + DAY14_STRIDE * y] = ' ';
            }
            grid[DAY14_WIDTH + DAY14_STRIDE * y] = '\n';
        }

        for (u32 i = 0; i < robots.count; ++i) {
            Day14Robot* r                    = &robots.items[i];
            r->x                             = i64_mod(r->x + r->vx, DAY14_WIDTH);
            r->y                             = i64_mod(r->y + r->vy, DAY14_HEIGHT);
            grid[r->x + DAY14_STRIDE * r->y] = 'X';
        }
        elapsed++;

        printf("-----------------------------------------------------------------------------------------------------------------\n");
        printf("%s\n", grid);
        printf("%llu\n", elapsed);
    }
}

internal DayResult day14(Arena* arena, Str input) {
    Vec_Day14Robot robots = VecAlloc(Day14Robot, arena, 512);
    i64            a = 0, b = 0, c = 0, d = 0;

    foreach (StrSplitIter, lines_iter, '\n', input) {
        Str          p_pair      = str_after_first_index('=', str_before_first_index(' ', lines_iter.item));
        StrSplitIter p_pair_iter = StrSplitIter_new(',', p_pair);
        i64          px          = str_parse_i64(p_pair_iter.item, 10);
        StrSplitIter_next(&p_pair_iter);
        i64 py = str_parse_i64(p_pair_iter.item, 10);

        Str          v_pair      = str_after_last_index('=', lines_iter.item);
        StrSplitIter v_pair_iter = StrSplitIter_new(',', v_pair);
        i64          vx          = str_parse_i64(v_pair_iter.item, 10);
        StrSplitIter_next(&v_pair_iter);
        i64 vy = str_parse_i64(v_pair_iter.item, 10);

        i64 testx = i64_mod(px + 100 * vx, DAY14_WIDTH);
        i64 testy = i64_mod(py + 100 * vy, DAY14_HEIGHT);

        if (testx < DAY14_WIDTH / 2 && testy < DAY14_HEIGHT / 2) a++;
        if (testx > DAY14_WIDTH / 2 && testy < DAY14_HEIGHT / 2) b++;
        if (testx < DAY14_WIDTH / 2 && testy > DAY14_HEIGHT / 2) c++;
        if (testx > DAY14_WIDTH / 2 && testy > DAY14_HEIGHT / 2) d++;

        *VecPush(robots) = (Day14Robot){
            .x  = px,
            .y  = py,
            .vx = vx,
            .vy = vy,
        };
    }

    if (DAY14_ENABLE_PRINT_MODE) {
        day14_print_pictures(arena, robots.slice);
    }

    i64 part1 = a * b * c * d;
    i64 part2 = 6446;  // determined by sifting through the printed results

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}