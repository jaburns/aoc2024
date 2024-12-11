#include "main.h"

#define DAY11_HASHARRAY_CAPACITY Kb(8)

internal i32 day11_count_decimal_digits(u64 b) {
    i32 digits = 0;
    while (b > 0) {
        b /= 10;
        digits++;
    }
    return digits;
}

internal void day11_split(i32 digits, u64 value, u64* a, u64* b) {
    digits   /= 2;
    u64 tens  = 1;
    while (--digits >= 0) tens *= 10;
    *a = value / tens;
    *b = value % tens;
}

internal void day11_blink(HashArray* hash0, HashArray* hash1) {
    foreach (HashArrayIter, it, hash0) {
        u64 value = *(u64*)it.key;
        u64 count = *(u64*)it.value;

        if (value == 0) {
            u64 target_value                              = 1;
            *(u64*)hasharray_entry(hash1, &target_value) += count;
        } else {
            i32 digits = day11_count_decimal_digits(value);
            if (digits & 1) {
                u64 target_value                              = value * 2024;
                *(u64*)hasharray_entry(hash1, &target_value) += count;
            } else {
                u64 a, b;
                day11_split(digits, value, &a, &b);
                *(u64*)hasharray_entry(hash1, &a) += count;
                *(u64*)hasharray_entry(hash1, &b) += count;
            }
        }
    }
}

internal void day11_blink_multiple(HashArray** hash0p, HashArray** hash1p, u32 count) {
    for (u32 i = 0; i < count; ++i) {
        hasharray_clear(*hash1p);
        day11_blink(*hash0p, *hash1p);
        Swap(HashArray*, *hash0p, *hash1p);
    }
}

internal u64 day11_count(HashArray* hash) {
    u64 result = 0;
    foreach (HashArrayIter, it, hash) {
        result += *(u64*)it.value;
    }
    return result;
}

internal DayResult day11(Arena* arena, Str input) {
    HashArray* hash0 = hasharray_alloc_with_cap(arena, sizeof(u64), sizeof(u64), DAY11_HASHARRAY_CAPACITY);
    HashArray* hash1 = hasharray_alloc_with_cap(arena, sizeof(u64), sizeof(u64), DAY11_HASHARRAY_CAPACITY);

    foreach (StrSplitWhitespaceIter, it, input) {
        u64 value = str_parse_u64(it.item, 10);
        ++*(u64*)hasharray_entry(hash0, &value);
    }

    day11_blink_multiple(&hash0, &hash1, 25);
    u64 part1 = day11_count(hash0);
    day11_blink_multiple(&hash0, &hash1, 50);
    u64 part2 = day11_count(hash0);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}