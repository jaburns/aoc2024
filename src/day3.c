#include "main.h"

#define IsDigit(x) (x >= '0' && x <= '9')

internal DayResult day3(Arena* arena, Str input) {
    u64  part1   = 0;
    u64  part2   = 0;
    bool enabled = true;

    u8* walk = (u8*)input.items;
    u8* end  = walk + input.count;
    while (walk < end) {
        u8x16 chunk = u8x16_load(walk);
        u8x16 eq_m  = u8x16_ceq(chunk, u8x16_dup('m'));
        u8x16 eq_d  = u8x16_ceq(chunk, u8x16_dup('d'));

        bool has_m = u8x16_maxv(eq_m);
        bool has_d = u8x16_maxv(eq_d);

        if (!has_m && !has_d) {
            walk += 16;
            continue;
        }

        u8x8 m_idx_v = u16x8_shrn(u16x8_from_u8x16(eq_m), 4);
        u64  m_idx   = u64_count_leading_zeroes(u64_bit_reverse(u64_from_u8x8(m_idx_v))) / 4 + (has_m ? 0 : 1000);
        u8x8 d_idx_v = u16x8_shrn(u16x8_from_u8x16(eq_d), 4);
        u64  d_idx   = u64_count_leading_zeroes(u64_bit_reverse(u64_from_u8x8(d_idx_v))) / 4 + (has_d ? 0 : 1000);

        if (d_idx < m_idx) {
            walk += d_idx + 1;

            if (*walk++ != 'o') continue;
            char ch = *walk++;
            if (ch == '(') {
                if (*walk++ != ')') continue;
                enabled = true;
            } else if (ch == 'n') {
                if (*walk++ != '\'') continue;
                if (*walk++ != 't') continue;
                if (*walk++ != '(') continue;
                if (*walk++ != ')') continue;
                enabled = false;
            }
        } else {
            walk += m_idx + 1;

            if (*walk++ != 'u') continue;
            if (*walk++ != 'l') continue;
            if (*walk++ != '(') continue;

            u32 a = 0, b = 0;

            char ch = *walk++;
            if (!IsDigit(ch)) continue;
            a = ch - '0';

            ch = *walk++;
            if (ch == ',') goto parse_b;
            if (!IsDigit(ch)) continue;
            a = 10 * a + (ch - '0');

            ch = *walk++;
            if (ch == ',') goto parse_b;
            if (!IsDigit(ch)) continue;
            a = 10 * a + (ch - '0');

            if (*walk++ != ',') continue;

        parse_b: {}

            ch = *walk++;
            if (!IsDigit(ch)) continue;
            b = ch - '0';

            ch = *walk++;
            if (ch == ')') goto parse_c;
            if (!IsDigit(ch)) continue;
            b = 10 * b + (ch - '0');

            ch = *walk++;
            if (ch == ')') goto parse_c;
            if (!IsDigit(ch)) continue;
            b = 10 * b + (ch - '0');

            if (*walk++ != ')') continue;

        parse_c: {}

            u64 product  = a * b;
            part1       += product;
            if (enabled) part2 += product;
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}

#undef IsDigit