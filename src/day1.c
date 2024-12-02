#include "main.h"

#define MAX_LIST_LEN 1024
#define MIN_VALUE    10000
#define MAX_VALUE    100000
#define VALUE_RANGE  (MAX_VALUE - MIN_VALUE)

internal i32 day1_parse_number(u8* string) {
    u8x8  digit_mask   = u8x8_splat(0x0F);
    u32x4 digit_values = {10000, 1000, 100, 10};

    u8x8  digits = u8x8_and(u8x8_load(string), digit_mask);
    u32x4 hi_4   = u16x4_widen(u16x8_get_low(u8x8_widen(digits)));
    u32   value  = u32x4_add_across(u32x4_mul(digit_values, hi_4));
    return (i32)(value + u8x8_get_lane(digits, 4));
}

internal DayResult day1(Arena* arena, Str input) {
    u8* left_counts  = arena_alloc(arena, VALUE_RANGE * sizeof(u8));
    u8* right_counts = arena_alloc(arena, VALUE_RANGE * sizeof(u8));

    u32 list_elem_count = 0;
    {
        u8* read     = (u8*)input.items;
        u8* read_end = read + input.count;

        while (read < read_end) {
            i32 left  = day1_parse_number(read);
            read     += 8;
            left_counts[left - MIN_VALUE]++;

            i32 right  = day1_parse_number(read);
            read      += 6;
            right_counts[right - MIN_VALUE]++;

            ++list_elem_count;
        }
    }

    i32 part1 = 0;
    i32 part2 = 0;

    u8* left_walk  = left_counts;
    u8* right_walk = right_counts;

    i32 left_value, left_remaining   = 0;
    i32 right_value, right_remaining = 0;

    for (u32 c = 0; c < list_elem_count; ++c) {
        if (left_remaining > 0) {
            --left_remaining;
        } else {
            while (!u32x4_max_across(u32x4_load((u32*)left_walk))) {
                left_walk += 16;
            }
            for (;;) {
                u8 left_count = *left_walk;
                if (left_count) {
                    left_value     = (left_walk - left_counts) + MIN_VALUE;
                    left_remaining = left_count - 1;
                    left_walk++;
                    break;
                }
                left_walk++;
            }
        }

        if (right_remaining > 0) {
            --right_remaining;
        } else {
            while (!u32x4_max_across(u32x4_load((u32*)right_walk))) {
                right_walk += 16;
            }
            for (;;) {
                u8 right_count = *right_walk;
                if (right_count) {
                    right_value     = (right_walk - right_counts) + MIN_VALUE;
                    right_remaining = right_count - 1;
                    right_walk++;
                    break;
                }
                right_walk++;
            }
        }

        part1 += Abs(right_value - left_value);
        part2 += left_value * right_counts[left_value - MIN_VALUE];
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}