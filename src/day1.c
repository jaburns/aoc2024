#include "main.h"

#define MAX_LIST_LEN 1024
#define MAX_VALUE    100000

internal i32 day1_parse_number(u8* string) {
    u8x8  digit_mask   = u8x8_splat(0x0F);
    u32x4 digit_values = {10000, 1000, 100, 10};

    u8x8  digits = u8x8_and(u8x8_load(string), digit_mask);
    u32x4 hi_4   = u16x4_widen(u16x8_get_low(u8x8_widen(digits)));
    u32   value  = u32x4_add_across(u32x4_mul(digit_values, hi_4));
    return (i32)(value + u8x8_get_lane(digits, 4));
}

internal DayResult day1(Arena* arena, Str input) {
    i32* left_list    = arena_alloc_nz(arena, MAX_LIST_LEN * sizeof(i32));
    i32* right_list   = arena_alloc_nz(arena, MAX_LIST_LEN * sizeof(i32));
    u16* right_counts = arena_alloc(arena, MAX_VALUE * sizeof(u16));

    u32 list_elem_count;
    {
        u8* read     = (u8*)input.items;
        u8* read_end = read + input.count;

        i32* left_write  = left_list;
        i32* right_write = right_list;

        while (read < read_end) {
            *left_write++  = day1_parse_number(read);
            read          += 8;

            i32 right      = day1_parse_number(read);
            *right_write++ = right;
            right_counts[right]++;

            read += 6;
        }

        list_elem_count = left_write - left_list;
    }

    ArraySort(left_list, list_elem_count, sort_fn_i32_asc);
    ArraySort(right_list, list_elem_count, sort_fn_i32_asc);

    i32 part1 = 0;
    {
        i32* left_read  = left_list;
        i32* right_read = right_list;

        u32 chunks = list_elem_count / 4;

        DebugAssert(list_elem_count == chunks * 4);

        for (u32 i = 0; i < chunks; ++i) {
            i32x4 left  = i32x4_load(left_read);
            i32x4 right = i32x4_load(right_read);

            part1 += i32x4_add_across(i32x4_abs(i32x4_sub(left, right)));

            left_read  += 4;
            right_read += 4;
        }
    }

    i32 part2 = 0;
    for (u32 i = 0; i < list_elem_count; ++i) {
        i32 val  = left_list[i];
        part2   += val * right_counts[val];
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}