#include "main.h"

#define MAX_ROWS    1024
#define MAX_COLUMNS 8

internal DayResult day2(Arena* arena, Str input) {
    u8* data      = arena_alloc(arena, MAX_COLUMNS * MAX_ROWS * sizeof(u8));
    u8* lengths   = arena_alloc(arena, MAX_ROWS * sizeof(u8));
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

                // simd parse first 4 numbers if they're all 2 digits
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

            i8x8 mask       = LENGTH_MASKS[len];
            i8x8 row        = i8x8_load(walk);
            i8x8 shifted    = i8x8_extract(row, i8x8_splat(0), 1);
            i8x8 diffs      = i8x8_and(mask, i8x8_sub(shifted, row));
            i8x8 first_diff = i8x8_splat(i8x8_get_lane(diffs, 0));
            i8x8 extended   = i8x8_select(mask, diffs, first_diff);

            i8 min = i8x8_min_across(extended);
            i8 max = i8x8_max_across(extended);

            if (max <= -1 && min >= -3 || min >= 1 && max <= 3) {
                part1++;
            }

            walk += 8;
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = 0;
    return result;
}