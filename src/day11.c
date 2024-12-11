#include "main.h"

#define DAY11_HASHARRAY_CAPACITY 4096

internal u32 day11_count_decimal_digits(u64 b) {
    u32 digits = 0;
    while (b > 0) {
        b /= 10;
        digits++;
    }
    return digits;
}

internal void day11_split(u32 digits, u64 value, u64* a, u64* b) {
    u64 prod = 1;
    while (digits > 0) {
        prod *= 10;
        digits--;
    }
    *a = value / prod;
    *b = value % prod;
}

internal void day11_blink(HashArray* hash0, HashArray* hash1) {
    foreach (HashArrayIter, it, hash0) {
        u64 value = *(u64*)it.key;
        u64 count = *(u64*)it.value;

        if (value == 0) {
            u64 target_value                              = 1;
            *(u64*)hasharray_entry(hash1, &target_value) += count;
        } else {
            u32 digits = day11_count_decimal_digits(value);
            if (digits & 1) {
                u64 target_value                              = value * 2024;
                *(u64*)hasharray_entry(hash1, &target_value) += count;
            } else {
                u64 a, b;
                day11_split(digits / 2, value, &a, &b);
                *(u64*)hasharray_entry(hash1, &a) += count;
                *(u64*)hasharray_entry(hash1, &b) += count;
            }
        }
    }
}

internal DayResult day11(Arena* arena, Str input) {
    HashArray* hash0 = hasharray_alloc(arena, sizeof(u64), sizeof(u64), DAY11_HASHARRAY_CAPACITY);
    HashArray* hash1 = hasharray_alloc(arena, sizeof(u64), sizeof(u64), DAY11_HASHARRAY_CAPACITY);

    foreach (StrSplitWhitespaceIter, it, input) {
        u64 value = str_parse_u64(it.item, 10);
        ++*(u64*)hasharray_entry(hash0, &value);
    }

    for (u32 i = 0; i < 25; ++i) {
        hasharray_clear(hash1);
        day11_blink(hash0, hash1);
        Swap(HashArray*, hash0, hash1);
    }

    u64 part1 = 0;
    foreach (HashArrayIter, it, hash0) {
        part1 += *(u64*)it.value;
    }

    for (u32 i = 0; i < 50; ++i) {
        hasharray_clear(hash1);
        day11_blink(hash0, hash1);
        Swap(HashArray*, hash0, hash1);
    }

    u64 part2 = 0;
    foreach (HashArrayIter, it, hash0) {
        part2 += *(u64*)it.value;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}