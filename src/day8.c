#include "main.h"

#if TEST_INPUTS
#define DAY8_SIZE 12
#else
#define DAY8_SIZE 50
#endif

#define DAY8_MAX_ANTENNAE_PER_FREQ 32
#define DAY8_SLOTS                 75

enumdef(Day8Flag, u8){
    DAY8_FLAG_PART_1 = 1,
    DAY8_FLAG_PART_2 = 2,
};

structdef(Day8Frequency) {
    StaticVec(ivec2, DAY8_MAX_ANTENNAE_PER_FREQ) coords;
};

internal i64 day8_place_flag(Day8Flag* placed, Day8Flag flag, ivec2 target, bool* in_bounds) {
    *in_bounds = target.x >= 0 && target.x < DAY8_SIZE && target.y >= 0 && target.y < DAY8_SIZE;
    if (*in_bounds) {
        u32 idx = target.x + DAY8_SIZE * target.y;
        if ((placed[idx] & flag) == 0) {
            placed[idx] |= flag;
            return 1;
        }
    }
    return 0;
}

internal DayResult day8(Arena* arena, Str input) {
    Day8Frequency* slots  = ArenaAlloc(Day8Frequency, arena, DAY8_SLOTS);
    Day8Flag*      placed = ArenaAlloc(Day8Flag, arena, DAY8_SIZE * DAY8_SIZE);

    i32 y = 0;
    foreach (StrSplitIter, line, '\n', input) {
        for (i32 x = 0; x < line.item.count; ++x) {
            if (line.item.items[x] < '0') continue;
            u8 freq = (u8)(line.item.items[x] - '0');

            *StaticVecPush(slots[freq].coords) = ivec2(x, y);
        }
        y++;
    }

    i64 part1 = 0;
    i64 part2 = 0;

    for (u32 slot = 0; slot < DAY8_SLOTS; ++slot) {
        if (slots[slot].coords.count == 0) continue;

        for (u32 i = 0; i < slots[slot].coords.count - 1; ++i) {
            for (u32 j = i + 1; j < slots[slot].coords.count; ++j) {
                ivec2 a = slots[slot].coords.items[i];
                ivec2 b = slots[slot].coords.items[j];

                ivec2 delta  = ivec2_sub(b, a);
                ivec2 ndelta = ivec2_negate(delta);
                bool  in_bounds;

                part1 += day8_place_flag(placed, DAY8_FLAG_PART_1, ivec2_add(b, delta), &in_bounds);
                part1 += day8_place_flag(placed, DAY8_FLAG_PART_1, ivec2_add(a, ndelta), &in_bounds);

                in_bounds    = true;
                ivec2 target = b;
                while (in_bounds) {
                    part2  += day8_place_flag(placed, DAY8_FLAG_PART_2, target, &in_bounds);
                    target  = ivec2_add(target, delta);
                }
                in_bounds = true;
                target    = a;
                while (in_bounds) {
                    part2  += day8_place_flag(placed, DAY8_FLAG_PART_2, target, &in_bounds);
                    target  = ivec2_add(target, ndelta);
                }
            }
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}
