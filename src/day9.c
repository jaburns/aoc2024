#include "main.h"

internal void day9_part1_compact(u16* start, u16* end) {
    for (;;) {
        while (*start) start++;
        while (!*end) end--;

        if (start >= end) break;

        *start = *end;
        *end   = 0;
    }
}

internal void day9_part2_compact(u16* start, u16* end) {
    u16* src_start = end;

    while (!*src_start) src_start--;
    u16  src_id  = *src_start;
    u16* src_end = src_start;
    while (*--src_start == src_id);
    u64 src_size = src_end - src_start;

    for (;;) {
        u16* seek = start;

        for (;;) {
            while (*seek) seek++;
            if (seek >= src_start) break;

            u16* hole_seek = seek;
            while (!*hole_seek) hole_seek++;

            if (hole_seek - seek >= src_size) {
                memcpy(seek, (src_start + 1), src_size * sizeof(u16));
                memset((src_start + 1), 0, src_size * sizeof(u16));
                break;
            }

            seek = hole_seek;
        }

        if (--src_id == 0) return;

        while (*src_start != src_id) src_start--;
        src_end = src_start;
        while (*--src_start == src_id);
        src_size = src_end - src_start;
    }
}

internal u64 day9_checksum(u16* start, u16* end) {
    u64 result = 0;
    u64 i      = 0;

    do {
        u16 val = *start++;
        if (val) result += i * (val - 1);
        i++;
    } while (start < end);

    return result;
}

internal DayResult day9(Arena* arena, Str input) {
    u16* data  = ArenaAlloc(u16, arena, 10 * input.count);
    u16* data2 = ArenaAlloc(u16, arena, 10 * input.count);

    u16* end     = data;
    u16  file_id = 1;
    for (u16 i = 0;;) {
        u16 value = (i16)(input.items[i] - '0');
        for (u16 j = 0; j < value; ++j) {
            *end++ = file_id;
        }
        file_id++;

        if (++i >= input.count) break;

        value  = (i16)(input.items[i] - '0');
        end   += value;

        if (++i >= input.count) break;
    }

    ArrayCopy(data2, data, 0, 10 * input.count);

    day9_part1_compact(data, end);
    u64 part1 = day9_checksum(data, end);

    u16* end2 = data2 + (end - data);
    day9_part2_compact(data2, end2);
    u64 part2 = day9_checksum(data2, end2);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}