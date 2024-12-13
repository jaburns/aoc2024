#include "main.h"

internal u64 day13_solve(f64 ax, f64 ay, f64 bx, f64 by, f64 cx, f64 cy, f64 offset) {
    f64 px = cx + offset;
    f64 py = cy + offset;

    f64 fy = (px / ax * ay - py) / (bx / ax * ay - by);
    f64 fx = (px - bx * fy) / ax;

    f64 x = round(fx);
    f64 y = round(fy);

    return x * ax + y * bx == px && x * ay + y * by == py
               ? 3 * (u64)x + (u64)y
               : 0;
}

internal DayResult day13(Arena* arena, Str input) {
    u64 part1 = 0;
    u64 part2 = 0;

    StrSplitIter lines_iter = StrSplitIter_new('\n', input);
    while (!lines_iter.done) {
        Str sax = str_before_first_index(',', str_after_first_index('+', lines_iter.item));
        Str say = str_after_last_index('+', lines_iter.item);
        f64 ax  = (f64)str_parse_u32(sax, 10);
        f64 ay  = (f64)str_parse_u32(say, 10);
        StrSplitIter_next(&lines_iter);

        Str sbx = str_before_first_index(',', str_after_first_index('+', lines_iter.item));
        Str sby = str_after_last_index('+', lines_iter.item);
        f64 bx  = (f64)str_parse_u32(sbx, 10);
        f64 by  = (f64)str_parse_u32(sby, 10);
        StrSplitIter_next(&lines_iter);

        Str scx = str_before_first_index(',', str_after_first_index('=', lines_iter.item));
        Str scy = str_after_last_index('=', lines_iter.item);
        f64 cx  = (f64)str_parse_u32(scx, 10);
        f64 cy  = (f64)str_parse_u32(scy, 10);
        StrSplitIter_next(&lines_iter);
        StrSplitIter_next(&lines_iter);

        part1 += day13_solve(ax, ay, bx, by, cx, cy, 0.0);
        part2 += day13_solve(ax, ay, bx, by, cx, cy, 10000000000000.0);
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}