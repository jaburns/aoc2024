#include "main.h"

#define DAY5_MAX_UPDATES    512
#define DAY5_MAX_PAGES      128
#define DAY5_MAX_UPDATE_LEN 32

//  Part 1: 7307
//  Part 2: 4713

internal void day5_quicksort(u8* arr, size_t len, i8* rules) {
    size_t  stack_base[2 * DAY5_MAX_UPDATE_LEN];
    size_t* stack   = stack_base;
    size_t  halflen = len / 2;  // don't bother sorting the right half of the array

    *stack++ = 0;
    *stack++ = len - 1;

    while (stack > stack_base) {
        size_t end   = *--stack;
        size_t start = *--stack;

        u8*    pivot = arr + end;
        size_t i     = start;
        for (size_t j = start; j < end; j++) {
            u8 a = arr[j];
            u8 b = *pivot;
            if (rules[a * DAY5_MAX_PAGES + b] < 0) {
                if (i != j) {
                    Swap(u8, arr[i], arr[j]);
                }
                i++;
            }
        }
        if (i != end) {
            Swap(u8, pivot[0], arr[i]);
        }

        if (i > start + 1 && start <= halflen) {
            *stack++ = start;
            *stack++ = i - 1;
        }
        i++;
        if (i < end && i <= halflen) {
            *stack++ = i;
            *stack++ = end;
        }
    }
}

internal DayResult day5(Arena* arena, Str input) {
    i8* rules   = arena_alloc(arena, DAY5_MAX_PAGES * DAY5_MAX_PAGES * sizeof(u8));
    u8* updates = arena_alloc(arena, DAY5_MAX_UPDATE_LEN * DAY5_MAX_UPDATES * sizeof(u8));

    i64 part1 = 0;
    i64 part2 = 0;

    u8* walk      = (u8*)input.items;
    u8* input_end = walk + input.count;

    // assumes an even number of page rules
    while (*walk != '\n') {
        u8x16 rule_pair = u8x16_load(walk);
        u8x16 digits    = u8x16_and(rule_pair, u8x16_splat(0x0F));
        u8x16 placed    = u8x16_mul(digits, (u8x16){10, 1, 0, 10, 1, 0, 10, 1, 0, 10, 1, 0, 0, 0, 0, 0});
        u8x16 added     = u8x16_add(placed, u8x16_extract(placed, u8x16_splat(0), 1));

        u8 before0 = u8x16_get_lane(added, 0);
        u8 after0  = u8x16_get_lane(added, 3);
        u8 before1 = u8x16_get_lane(added, 6);
        u8 after1  = u8x16_get_lane(added, 9);

        rules[before0 * DAY5_MAX_PAGES + after0] = 1;
        rules[after0 * DAY5_MAX_PAGES + before0] = -1;
        rules[before1 * DAY5_MAX_PAGES + after1] = 1;
        rules[after1 * DAY5_MAX_PAGES + before1] = -1;

        walk += 12;
    }

    walk++;

    u8* cur_update_write       = updates + 1;
    u8* cur_update_write_start = updates + 1;
    u8* cur_update_write_len   = updates;

    while (walk < input_end) {
        u8x16 chunk          = u8x16_load(walk);
        u8    any_terminator = u8x16_max_across(u8x16_less_than(chunk, u8x16_splat('\n' + 1)));

        if (!any_terminator) {
            u8x16 digits = u8x16_and(chunk, u8x16_splat(0x0F));
            u8x16 placed = u8x16_mul(digits, (u8x16){10, 1, 0, 10, 1, 0, 10, 1, 0, 10, 1, 0, 10, 1, 0, 0});
            u8x16 added  = u8x16_add(placed, u8x16_extract(placed, u8x16_splat(0), 1));

            *cur_update_write++ = u8x16_get_lane(added, 0);
            *cur_update_write++ = u8x16_get_lane(added, 3);
            *cur_update_write++ = u8x16_get_lane(added, 6);
            *cur_update_write++ = u8x16_get_lane(added, 9);
            *cur_update_write++ = u8x16_get_lane(added, 12);

            walk += 15;
            continue;
        }

        for (;;) {
            u8 value            = (walk[0] & 0x0F) * 10 + (walk[1] & 0x0F);
            *cur_update_write++ = value;

            walk += 3;
            if (*(walk - 1) != ',') {
                *cur_update_write_len   = cur_update_write - cur_update_write_start;
                cur_update_write_start += DAY5_MAX_UPDATE_LEN;
                cur_update_write_len   += DAY5_MAX_UPDATE_LEN;
                cur_update_write        = cur_update_write_start;
                break;
            }
        }
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
        day5_quicksort(update, len, rules);
        part2 += update[len / 2];
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}