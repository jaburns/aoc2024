#include "main.h"

#define DAY6_ENTRIES_CAPACITY     1024
#define DAY6_ENTRY_ITEMS_CAPACITY 32

enumdef(Day7Op, u8){
    DAY7_PLUS,
    DAY7_TIMES,
    DAY7_CONCAT,
};

structdef(Day7Entry) {
    u64     total;
    Vec_u16 items;
};
DefArrayTypes(Day7Entry);

readonly_global u64 DAY7_TENS[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL,
};

internal u32 day7_count_decimal_digits(u64 b) {
    u32 digits = 0;
    while (b > 0) {
        b /= 10;
        digits++;
    }
    return digits;
}

internal u64 day7_op_exec(Day7Op op, u64 a, u64 b) {
    switch (op) {
        case DAY7_PLUS: {
            return b == 1 ? a : a + b;  // multiply/add are swapped when b = 1
        }
        case DAY7_TIMES: {
            return b == 1 ? a + 1 : a * b;
        }
        case DAY7_CONCAT: {
            return a * DAY7_TENS[day7_count_decimal_digits(b)] + b;
        }
    }
    return 0;
}

internal void day7_perm_from_idx(Day7Op out_perm[], u32 base, u32 idx, u32 entry_item_count) {
    u32 i = entry_item_count - 1;
    while (i-- > 0) {
        out_perm[i]  = idx % base;
        idx         /= base;
    }
}

internal bool day7_eval_permutation(Day7Entry* entry, Day7Op perm[]) {
    u64 number = entry->items.items[0];
    for (u32 i = 1; i < entry->items.count; ++i) {
        number = day7_op_exec(perm[i - 1], number, entry->items.items[i]);
    }
    return number == entry->total;
}

internal u64 day7_count_matches(Slice_Day7Entry entries, u32 base) {
    u64 result = 0;

    for (u32 i = 0; i < entries.count; ++i) {
        Day7Entry* entry = &entries.items[i];

        u64 perms = (u64)pow(base, entry->items.count - 1);

        Day7Op perm[DAY6_ENTRY_ITEMS_CAPACITY] = {0};

        for (u32 j = 0; j < perms; ++j) {
            day7_perm_from_idx(perm, base, j, entry->items.count);
            if (day7_eval_permutation(entry, perm)) {
                result += entry->total;
                break;
            }
        }
    }

    return result;
}

internal DayResult day7(Arena* arena, Str input) {
    Vec_Day7Entry entries = VecAlloc(Day7Entry, arena, DAY6_ENTRIES_CAPACITY);

    foreach (StrSplitIter, lines, '\n', input) {
        StrSplitIter by_colon = StrSplitIter_new(':', lines.item);

        Day7Entry entry = (Day7Entry){
            .total = str_parse_u64(by_colon.item, 10),
            .items = VecAlloc(u16, arena, DAY6_ENTRY_ITEMS_CAPACITY),
        };

        StrSplitIter_next(&by_colon);
        Str parts = by_colon.item;

        foreach (StrSplitIter, num, ' ', str_trim(parts)) {
            *VecPush(entry.items) = str_parse_u32(num.item, 10);
        }

        *VecPush(entries) = entry;
    }

    i64 part1 = day7_count_matches(entries.slice, 2);
    i64 part2 = day7_count_matches(entries.slice, 3);

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}