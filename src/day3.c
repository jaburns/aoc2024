#include "main.h"

#define DAY3_CMD_DONT_VALUE 0x0000292874276E6F  // "on't()"
#define DAY3_CMD_DONT_MASK  0x0000FFFFFFFFFFFF
#define DAY3_CMD_DO_VALUE   0x000000000029286F  // "o()"
#define DAY3_CMD_DO_MASK    0x0000000000FFFFFF
#define DAY3_CMD_MUL_VALUE  0x0000000000286C75  // "ul("
#define DAY3_CMD_MUL_MASK   0x0000000000FFFFFF

internal i32 day3_parse_number(u8** walk, char terminator) {
    char ch = *(*walk)++;
    if (ch < '0' || ch > '9') return -1;
    i32 a = ch - '0';

    ch = *(*walk)++;
    if (ch == terminator) return a;
    if (ch < '0' || ch > '9') return -1;
    a = 10 * a + (ch - '0');

    ch = *(*walk)++;
    if (ch == terminator) return a;
    if (ch < '0' || ch > '9') return -1;
    a = 10 * a + (ch - '0');

    ch = *(*walk)++;
    if (ch != terminator) return -1;
    return a;
}

internal DayResult day3(Arena* arena, Str input) {
    u64  part1   = 0;
    u64  part2   = 0;
    bool enabled = true;

    u8* walk = (u8*)input.items;
    u8* end  = walk + input.count;
    while (walk < end) {
        u8x16 chunk = u8x16_load(walk);
        u8x16 eq_m  = u8x16_equal(chunk, u8x16_splat('m'));
        u8x16 eq_d  = u8x16_equal(chunk, u8x16_splat('d'));

        bool has_m = u8x16_max_across(eq_m);
        bool has_d = u8x16_max_across(eq_d);

        if (!has_m && !has_d) {
            walk += 16;
            continue;
        }

        u8x8 m_idx_v = u16x8_shrn(u16x8_from_u8x16(eq_m), 4);
        u64  m_idx   = u64_count_leading_zeroes(u64_bit_reverse(u64_from_u8x8(m_idx_v))) / 4;
        u8x8 d_idx_v = u16x8_shrn(u16x8_from_u8x16(eq_d), 4);
        u64  d_idx   = u64_count_leading_zeroes(u64_bit_reverse(u64_from_u8x8(d_idx_v))) / 4;

        if (!has_m) m_idx |= 0x8000;
        if (!has_d) d_idx |= 0x8000;

        if (d_idx < m_idx) {
            walk += d_idx + 1;

            u64 word;
            memcpy(&word, walk, sizeof(u64));

            if ((word & DAY3_CMD_DONT_MASK) == DAY3_CMD_DONT_VALUE) {
                walk    += 6;
                enabled  = false;
            } else if ((word & DAY3_CMD_DO_MASK) == DAY3_CMD_DO_VALUE) {
                walk    += 3;
                enabled  = true;
            }
        } else {
            walk += m_idx + 1;

            u64 word;
            memcpy(&word, walk, sizeof(u64));

            if ((word & DAY3_CMD_MUL_MASK) != DAY3_CMD_MUL_VALUE) continue;

            walk += 3;

            i32 a = day3_parse_number(&walk, ',');
            if (a < 0) continue;
            i32 b = day3_parse_number(&walk, ')');
            if (b < 0) continue;

            i64 product  = a * b;
            part1       += product;
            if (enabled) part2 += product;
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}