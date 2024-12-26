#include "main.h"

structdef(Day25Object) {
    u8x16 vecs[2];
};
DefArrayTypes(Day25Object);

internal DayResult day25(Arena* arena, Str input) {
    Vec_Day25Object locks = VecAlloc(Day25Object, arena, 256);
    Vec_Day25Object keys  = VecAlloc(Day25Object, arena, 256);

    u8x16 overlap = u8x16_splat((u8)'#' + (u8)'#');

    for (u32 i = 0; i < input.count; i += 43) {
        Day25Object* new_obj;
        if (input.items[i] == '#') {
            new_obj = VecPush(locks);
        } else {
            new_obj = VecPush(keys);
        }
        memcpy(new_obj, &input.items[i + 6], 32);
    }

    u64 part1 = 0;

    for (u32 i = 0; i < keys.count; ++i) {
        Day25Object* key = &keys.items[i];
        for (u32 j = 0; j < locks.count; ++j) {
            Day25Object* lock = &locks.items[j];

            u8x16 eq0 = u8x16_equal(u8x16_add(key->vecs[0], lock->vecs[0]), overlap);
            u8x16 eq1 = u8x16_equal(u8x16_add(key->vecs[1], lock->vecs[1]), overlap);

            bool no_fit  = u8x16_max_across(eq0) || u8x16_max_across(eq1);
            part1       += !no_fit;
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = 1;
    return result;
}