#include "main.h"

#define BRUTE_FORCE 0

#define MAX_ROWS    Kb(512)
#define MAX_COLUMNS 1024

#if BIGBOY_INPUTS

internal DayResult day2(Arena* arena, Str input) {
    u32 part1 = 0;
    u32 part2 = 0;

    Panic("TODO: implement day 2 bigboy");

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}

#else

internal DayResult day2(Arena* arena, Str input) {
    u8* data      = arena_alloc_nz(arena, MAX_COLUMNS * MAX_ROWS * sizeof(u8));
    u8* lengths   = arena_alloc_nz(arena, MAX_ROWS * sizeof(u8));
    u32 row_count = 0;

    {
        u8x16 SPACES_MASK = {0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0, 0, 0, 0};
        u8x16 TENS_VALS   = {0x0A, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x0A, 0, 0, 0, 0, 0, 0};

        char* walk      = input.items;
        char* end       = input.items + input.count;
        u32   cur_row   = 0;
        u32   cur_idx   = 0;
        u8*   write_len = lengths;

        while (walk < end) {
            char ch = *walk++;
            if (ch >= '0') {
                u8 val = ch - '0';
                ch     = *walk;
                if (ch >= '0') {
                    walk++;
                    val = 10 * val + (ch - '0');
                }
                data[cur_idx++] = val;
            } else if (ch == '\n') {
                *write_len++ = cur_idx - cur_row * MAX_COLUMNS;
                cur_row++;
                cur_idx = cur_row * MAX_COLUMNS;

                u8x16 first_4 = u8x16_load((u8*)walk);
                if (u8x16_max_across(u8x16_and(first_4, SPACES_MASK)) == ' ') {
                    u8x16 digits = u8x16_sub(first_4, u8x16_splat('0'));
                    u8x16 tens   = u8x16_mul(digits, TENS_VALS);
                    u8x16 values = u8x16_add(tens, u8x16_extract(digits, digits, 1));

                    data[cur_idx++]  = u8x16_get_lane(values, 0);
                    data[cur_idx++]  = u8x16_get_lane(values, 3);
                    data[cur_idx++]  = u8x16_get_lane(values, 6);
                    data[cur_idx++]  = u8x16_get_lane(values, 9);
                    walk            += 12;
                }
            }
        }

        *write_len++ = cur_idx - cur_row * MAX_COLUMNS;
        row_count    = cur_row + 1;
    }

    u32 part1 = 0;
    u32 part2 = 0;
    {
        i8x8 LENGTH_MASKS[9] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0xFF, 0, 0, 0, 0, 0, 0, 0},
            {0xFF, 0xFF, 0, 0, 0, 0, 0, 0},
            {0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0},
            {0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0},
            {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, 0},
            {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0},
            {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0},
        };

        i8* walk = (i8*)data;
        u8* lens = lengths;
        for (u32 i = 0; i < row_count; ++i) {
            u8 len = *lens++;

            i8x8 mask = LENGTH_MASKS[len];
            i8x8 row  = i8x8_load(walk);

            i8x8 shifted    = i8x8_extract(row, i8x8_splat(0), 1);
            i8x8 diffs      = i8x8_and(mask, i8x8_sub(shifted, row));
            i8x8 first_diff = i8x8_splat(i8x8_get_lane(diffs, 0));
            i8x8 extended   = i8x8_select(mask, diffs, first_diff);

            i8 min = i8x8_min_across(extended);
            i8 max = i8x8_max_across(extended);

            if (min >= -3 && max <= -1 || min >= 1 && max <= 3) {
                part1++;
                part2++;
                walk += 8;
                continue;
            }

#if BRUTE_FORCE
            u64 start_idx = 0;
            u64 end_idx   = len;
#else
            i8x8 shifted_diffs = i8x8_extract(diffs, i8x8_splat(0), 1);
            u8x8 sign_changes  = i8x8_less_than(i8x8_xor(shifted_diffs, diffs), i8x8_splat(0));
            i8x8 abs_diff      = i8x8_abs(diffs);
            u8x8 bad_diffs     = u8x8_or(i8x8_less_than(abs_diff, i8x8_splat(1)), i8x8_greater_than(abs_diff, i8x8_splat(3)));
            u8x8 both_tests    = u8x8_or(sign_changes, bad_diffs);

            u64 tests64 = (u64)both_tests;
            if (!tests64) {
                walk += 8;
                continue;
            }

            u64 start_idx = u64_count_leading_zeroes(u64_bit_reverse(tests64)) / 8;
            u64 end_idx   = start_idx + 3;
            if (end_idx > len) end_idx = len;
#endif

            for (u32 j = start_idx; j < end_idx; ++j) {
                i8x8 mask = LENGTH_MASKS[len - 1];

                i8x8 after_cut = i8x8_extract(row, i8x8_splat(0), 1);
                u8x8 cut_mask  = (u8x8)(0xFFFFFFFFFFFFFFFF << (j * 8));
                i8x8 row1      = i8x8_select(cut_mask, after_cut, row);

                i8x8 shifted    = i8x8_extract(row1, i8x8_splat(0), 1);
                i8x8 diffs      = i8x8_and(mask, i8x8_sub(shifted, row1));
                i8x8 first_diff = i8x8_splat(i8x8_get_lane(diffs, 0));
                i8x8 extended   = i8x8_select(mask, diffs, first_diff);

                i8 min = i8x8_min_across(extended);
                i8 max = i8x8_max_across(extended);

                if (max <= -1 && min >= -3 || min >= 1 && max <= 3) {
                    part2++;
                    break;
                }
            }

            walk += 8;
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}

#endif