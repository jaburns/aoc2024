#include "main.h"

#if BIGBOY_INPUTS
#define DAY5_MAX_UPDATES    500000LLU
#define DAY5_MAX_PAGES      16384LLU
#define DAY5_MAX_UPDATE_LEN 8192LLU
typedef u16 Day5Int;
#else
#define DAY5_MAX_PAGES      128
#define DAY5_MAX_UPDATES    512
#define DAY5_MAX_UPDATE_LEN 32
typedef u8 Day5Int;
#endif

internal void day5_quicksort(Day5Int* arr, size_t len, i8* rules) {
    size_t  stack_base[2 * DAY5_MAX_UPDATE_LEN];
    size_t* stack   = stack_base;
    size_t  halflen = len / 2;  // don't bother sorting the right half of the array

    *stack++ = 0;
    *stack++ = len - 1;

    while (stack > stack_base) {
        size_t end   = *--stack;
        size_t start = *--stack;

        Day5Int* pivot = arr + end;
        size_t   i     = start;
        for (size_t j = start; j < end; j++) {
            Day5Int a = arr[j];
            Day5Int b = *pivot;
            if (rules[a * DAY5_MAX_PAGES + b] < 0) {
                if (i != j) {
                    Swap(Day5Int, arr[i], arr[j]);
                }
                i++;
            }
        }
        if (i != end) {
            Swap(Day5Int, pivot[0], arr[i]);
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
    i8*      rules   = arena_alloc(arena, DAY5_MAX_PAGES * DAY5_MAX_PAGES * sizeof(u8));
    Day5Int* updates = arena_alloc(arena, DAY5_MAX_UPDATE_LEN * DAY5_MAX_UPDATES * sizeof(u16));

    i64 part1 = 0;
    i64 part2 = 0;

    u8* walk      = (u8*)input.items;
    u8* input_end = walk + input.count;

#if BIGBOY_INPUTS  // ===================================================================================================

    while (*walk != '\n') {
        u8x8 chunk0 = u8x8_load(walk);
        walk++;
        u8x8 chunk1  = u8x8_load(walk);
        walk        += 9;

        u16x8 digits = u8x8_widen(u8x8_or(
            u8x8_and(chunk0, (u8x8){0x0F, 0x0F, 0x0F, 0x0F, 0, 0, 0, 0}),
            u8x8_and(chunk1, (u8x8){0, 0, 0, 0, 0x0F, 0x0F, 0x0F, 0x0F})
        ));

        u16x8 placed = u16x8_mul(digits, (u16x8){1000, 100, 10, 1, 1000, 100, 10, 1});
        u16   before = u16x4_add_across(u16x8_get_low(placed));
        u16   after  = u16x4_add_across(u16x8_get_high(placed));

        rules[before * DAY5_MAX_PAGES + after] = 1;
        rules[after * DAY5_MAX_PAGES + before] = -1;
    }

    walk++;

    u16* cur_update_write       = updates + 1;
    u16* cur_update_write_start = updates + 1;
    u16* cur_update_write_len   = updates;

    while (walk < input_end) {
        u8x8  chunk  = u8x8_load(walk);
        u16x8 digits = u8x8_widen(u8x8_and(chunk, (u8x8){0x0F, 0x0F, 0x0F, 0x0F, 0, 0, 0, 0}));
        u16   value  = u16x4_add_across(u16x4_mul(u16x8_get_low(digits), (u16x4){1000, 100, 10, 1}));

        *cur_update_write++ = value;

        walk += 4;
        if (*walk != ',') {
            *cur_update_write_len   = cur_update_write - cur_update_write_start;
            cur_update_write_start += DAY5_MAX_UPDATE_LEN;
            cur_update_write_len   += DAY5_MAX_UPDATE_LEN;
            cur_update_write        = cur_update_write_start;
        }
        walk++;
    }

#else  // NOT BIGBOY_INPUTS =============================================================================================

    // assumes an even number of page rules
    while (*walk != '\n') {
        u8x16 rule_pair = u8x16_load(walk);
        u8x16 digits    = u8x16_and(rule_pair, u8x16_splat(0x0F));
        u8x16 placed    = u8x16_mul(digits, (u8x16){10, 1, 0, 10, 1, 0, 10, 1, 0, 10, 1, 0, 0, 0, 0, 0});
        u8x16 added     = u8x16_add(placed, u8x16_shift_lanes(placed, 1));

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
        u64   terminator_idx = u8x16_nonzero_lane(u8x16_less_than(chunk, u8x16_splat('\n' + 1)));

        u8x16 digits = u8x16_and(chunk, u8x16_splat(0x0F));
        u8x16 td     = u8x16_swizzle(digits, (u8x16){3, 4, 6, 7, 9, 10, 12, 13, 0, 1, 255, 255, 255, 255, 255, 255});
        u8x16 placed = u8x16_mul(td, (u8x16){10, 1, 10, 1, 10, 1, 10, 1, 10, 1, 0, 0, 0, 0, 0, 0});
        u8x16 added  = u8x16_add_pairs(placed, u8x16_splat(0));

        *cur_update_write++ = u8x16_get_lane(added, 4);

        if (terminator_idx >= 15) {
            memcpy(cur_update_write, &added, 4);
            cur_update_write += 4;

            walk += 15;
            continue;
        }

        if (terminator_idx >= 3) *cur_update_write++ = u8x16_get_lane(added, 0);
        if (terminator_idx >= 6) *cur_update_write++ = u8x16_get_lane(added, 1);
        if (terminator_idx >= 9) *cur_update_write++ = u8x16_get_lane(added, 2);
        if (terminator_idx >= 12) *cur_update_write++ = u8x16_get_lane(added, 3);

        walk += terminator_idx + 1;

        *cur_update_write_len   = cur_update_write - cur_update_write_start;
        cur_update_write_start += DAY5_MAX_UPDATE_LEN;
        cur_update_write_len   += DAY5_MAX_UPDATE_LEN;
        cur_update_write        = cur_update_write_start;
    }

#endif  // BIGBOY_INPUTS ================================================================================================

    u64 total_updates = (cur_update_write_start - updates) / DAY5_MAX_UPDATE_LEN;

    for (u64 i = 0; i < total_updates; ++i) {
        Day5Int* update = updates + DAY5_MAX_UPDATE_LEN * i;
        Day5Int  len    = *update++;

        for (u32 j = 0; j < len - 1; ++j) {
            for (u32 k = j + 1; k < len; ++k) {
                Day5Int before = update[j];
                Day5Int after  = update[k];
                i8      check  = rules[before * DAY5_MAX_PAGES + after];
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