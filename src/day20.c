#include "main.h"

#if TEST_INPUTS
#define DAY20_SIZE       15
#define DAY20_MIN_SAVING 50
#else
#define DAY20_SIZE       141
#define DAY20_MIN_SAVING 100
#endif

internal DayResult day20(Arena* arena, Str input) {
    u32*      grid = ArenaAlloc(u32, arena, DAY20_SIZE * DAY20_SIZE);
    Vec_ivec2 path = VecAlloc(ivec2, arena, DAY20_SIZE * DAY20_SIZE);

    ivec2 start, end;

    u32 i = 0;
    for (u32 y = 0; y < DAY20_SIZE; ++y) {
        for (u32 x = 0; x < DAY20_SIZE; ++x) {
            switch (input.items[i++]) {
                case '.':
                    grid[x + y * DAY20_SIZE] = UINT32_MAX;
                    break;
                case 'S':
                    grid[x + y * DAY20_SIZE] = UINT32_MAX;
                    start                    = ivec2(x, y);
                    break;
                case 'E':
                    grid[x + y * DAY20_SIZE] = UINT32_MAX;
                    end                      = ivec2(x, y);
                    break;
            }
        }
        i++;
    }

    ivec2 pos  = start;
    u32   step = 1;
    for (;;) {
        grid[pos.x + pos.y * DAY20_SIZE] = step++;
        *VecPush(path)                   = pos;
        if (ivec2_eq(pos, end)) break;

        if (grid[pos.x + 1 + pos.y * DAY20_SIZE] == UINT32_MAX) {
            pos.x++;
            continue;
        }
        if (grid[pos.x - 1 + pos.y * DAY20_SIZE] == UINT32_MAX) {
            pos.x--;
            continue;
        }
        if (grid[pos.x + (pos.y + 1) * DAY20_SIZE] == UINT32_MAX) {
            pos.y++;
            continue;
        }
        if (grid[pos.x + (pos.y - 1) * DAY20_SIZE] == UINT32_MAX) {
            pos.y--;
            continue;
        }
        AssertUnreachable();
    }

    u32 part1 = 0;
    u32 part2 = 0;
    for (u32 i = 0; i < path.count; ++i) {
        ivec2 p   = path.items[i];
        i32   cur = grid[p.x + p.y * DAY20_SIZE];

        ivec2 delta;
        for (delta.y = -20; delta.y <= 20; ++delta.y) {
            i32 xrange = 20 - Abs(delta.y);
            for (delta.x = -xrange; delta.x <= xrange; ++delta.x) {
                ivec2 p1 = ivec2_add(p, delta);

                if (p1.x < 1 || p1.y < 1 || p1.x > DAY20_SIZE - 2 || p1.y > DAY20_SIZE - 2) continue;

                i32  skip_len = ivec2_manhattan(delta);
                i32  other    = grid[p1.x + p1.y * DAY20_SIZE];
                bool count    = other != 0 && other - cur >= DAY20_MIN_SAVING + skip_len;

                part1 += skip_len == 2 && count;
                part2 += count;
            }
        }
    }

#if TEST_INPUTS
    DebugAssert(part1 == 1);
    DebugAssert(part2 == 285);
#endif

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}
