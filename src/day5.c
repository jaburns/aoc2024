#include "main.h"

#define DAY5_MAX_UPDATES    512
#define DAY5_MAX_PAGES      128
#define DAY5_MAX_UPDATE_LEN 32

internal i32 day5_comparator(void* rules_v, const void* a_v, const void* b_v) {
    i8* rules = rules_v;
    u8  a     = *(u8*)a_v;
    u8  b     = *(u8*)b_v;
    return rules[a * DAY5_MAX_PAGES + b];
}

internal DayResult day5(Arena* arena, Str input) {
    i8* rules   = arena_alloc(arena, DAY5_MAX_PAGES * DAY5_MAX_PAGES * sizeof(u8));
    u8* updates = arena_alloc(arena, DAY5_MAX_UPDATE_LEN * DAY5_MAX_PAGES * sizeof(u8));

    i64 part1 = 0;
    i64 part2 = 0;

    u8* walk      = (u8*)input.items;
    u8* input_end = walk + input.count;

    while (*walk != '\n') {
        u8x8 pair          = u8x8_load(walk);
        u8x8 before_digits = u8x8_and(pair, (u8x8){0x0F, 0x0F, 0, 0, 0, 0, 0, 0});
        u8x8 after_digits  = u8x8_and(pair, (u8x8){0, 0, 0, 0x0F, 0x0F, 0, 0, 0});
        u8   before        = 10 * u8x8_get_lane(before_digits, 0) + u8x8_get_lane(before_digits, 1);
        u8   after         = 10 * u8x8_get_lane(after_digits, 3) + u8x8_get_lane(after_digits, 4);

        rules[before * DAY5_MAX_PAGES + after] = 1;
        rules[after * DAY5_MAX_PAGES + before] = -1;

        walk += 6;
    }

    walk++;

    u8* cur_update_write       = updates + 1;
    u8* cur_update_write_start = updates + 1;
    u8* cur_update_write_len   = updates;

    while (walk < input_end) {
        u8 value            = (walk[0] & 0x0F) * 10 + (walk[1] & 0x0F);
        *cur_update_write++ = value;

        walk += 2;
        if (*walk != ',') {
            *cur_update_write_len   = cur_update_write - cur_update_write_start;
            cur_update_write_start += DAY5_MAX_UPDATE_LEN;
            cur_update_write_len   += DAY5_MAX_UPDATE_LEN;
            cur_update_write        = cur_update_write_start;
        }
        walk++;
    }

    u32 total_updates = (cur_update_write_start - updates) / DAY5_MAX_UPDATE_LEN;

    for (u32 i = 0; i < total_updates; ++i) {
        u8* update = updates + DAY5_MAX_UPDATE_LEN * i;
        u8  len    = *update++;

        for (u32 j = 0; j < len - 1; ++j) {
            for (u32 k = j + 1; k < len; ++k) {
                u8 before = update[j];
                u8 after  = update[k];
                i8 check  = rules[before * DAY5_MAX_PAGES + after];
                if (check < 0) goto fail;
            }
        }

        part1 += update[len / 2];
        continue;

    fail:
        qsort_r(update, len, sizeof(u8), rules, day5_comparator);
        part2 += update[len / 2];
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}