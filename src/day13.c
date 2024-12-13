#include "main.h"

structdef(Day13Machine) {
    f64 ax, ay;
    f64 bx, by;
    f64 cx, cy;
};
DefArrayTypes(Day13Machine);

internal u64 day13_solve(Day13Machine* m, f64 offset) {
    f64 px = m->cx + offset;
    f64 py = m->cy + offset;

    f64 fy = (px / m->ax * m->ay - py) / (m->ay * m->bx / m->ax - m->by);
    f64 fx = px / m->ax - m->bx / m->ax * fy;

    f64 x = round(fx);
    f64 y = round(fy);

    if (x * m->ax + y * m->bx == px && x * m->ay + y * m->by == py) {
        return 3 * (u64)x + (u64)y;
    }
    return 0;
}

internal DayResult day13(Arena* arena, Str input) {
    u64 part1 = 0;
    u64 part2 = 0;

    Vec_Day13Machine machines = VecAlloc(Day13Machine, arena, 1024);

    StrSplitIter lines_iter = StrSplitIter_new('\n', input);
    while (!lines_iter.done) {
        Day13Machine* machine = VecPush(machines);

        Str ax      = str_before_first_index(',', str_after_first_index('+', lines_iter.item));
        Str ay      = str_after_last_index('+', lines_iter.item);
        machine->ax = (f64)str_parse_u32(ax, 10);
        machine->ay = (f64)str_parse_u32(ay, 10);
        StrSplitIter_next(&lines_iter);

        Str bx      = str_before_first_index(',', str_after_first_index('+', lines_iter.item));
        Str by      = str_after_last_index('+', lines_iter.item);
        machine->bx = (f64)str_parse_u32(bx, 10);
        machine->by = (f64)str_parse_u32(by, 10);
        StrSplitIter_next(&lines_iter);

        Str cx      = str_before_first_index(',', str_after_first_index('=', lines_iter.item));
        Str cy      = str_after_last_index('=', lines_iter.item);
        machine->cx = (f64)str_parse_u32(cx, 10);
        machine->cy = (f64)str_parse_u32(cy, 10);
        StrSplitIter_next(&lines_iter);
        StrSplitIter_next(&lines_iter);
    }

    for (u32 i = 0; i < machines.count; ++i) {
        part1 += day13_solve(&machines.items[i], 0.0);
        part2 += day13_solve(&machines.items[i], 10000000000000.0);
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}