#include "main.h"

structdef(ArrayWalker) {
    u8* array_start;
    u8* array_walk;
    i32 value;
    i32 value_copies;
};

#if BIGBOY_INPUTS

#define NUMBER_WIDTH 8
#define MAX_LIST_LEN 4000000
#define MIN_VALUE    10000000
#define MAX_VALUE    100000000
#define VALUE_RANGE  (MAX_VALUE - MIN_VALUE)

internal i32 day1_parse_number(u8* string) {
    u8x8  digit_mask      = u8x8_splat(0x0F);
    u32x4 digit_values_hi = {10000000, 1000000, 100000, 10000};
    u32x4 digit_values_lo = {1000, 100, 10, 1};

    u8x8 digits = u8x8_and(u8x8_load(string), digit_mask);

    u32x4 hi_4 = u16x4_widen(u16x8_get_low(u8x8_widen(digits)));
    u32x4 lo_4 = u16x4_widen(u16x8_get_high(u8x8_widen(digits)));

    return u32x4_add_across(u32x4_mul(digit_values_hi, hi_4)) +
           u32x4_add_across(u32x4_mul(digit_values_lo, lo_4));
}

#else

#define NUMBER_WIDTH 5
#define MAX_LIST_LEN 1000
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

#endif

internal void day1_walk_array_one_step(ArrayWalker* walker) {
    if (walker->value_copies > 0) {
        walker->value_copies--;
    } else {
        while (!u32x4_max_across(u32x4_load((u32*)walker->array_walk))) {
            walker->array_walk += 16;
        }
        for (;;) {
            u8 count = *walker->array_walk++;
            if (!count) continue;
            walker->value_copies = count - 1;
            walker->value        = walker->array_walk - 1 - walker->array_start + MIN_VALUE;
            break;
        }
    }
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
            read     += NUMBER_WIDTH + 3;
            // Assert(left_counts[left - MIN_VALUE] < 255);
            left_counts[left - MIN_VALUE]++;

            i32 right  = day1_parse_number(read);
            read      += NUMBER_WIDTH + 1;
            // Assert(right_counts[right - MIN_VALUE] < 255);
            right_counts[right - MIN_VALUE]++;

            ++list_elem_count;
        }
    }

    i64 part1 = 0;
    i64 part2 = 0;

    ArrayWalker left = (ArrayWalker){
        .array_start  = left_counts,
        .array_walk   = left_counts,
        .value_copies = 0,
    };
    ArrayWalker right = (ArrayWalker){
        .array_start  = right_counts,
        .array_walk   = right_counts,
        .value_copies = 0,
    };

    for (u32 c = 0; c < list_elem_count; ++c) {
        day1_walk_array_one_step(&left);
        day1_walk_array_one_step(&right);

        part1 += Abs(right.value - left.value);
        part2 += left.value * right_counts[left.value - MIN_VALUE];
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}