#include "main.h"

#define DAY6_ENTRIES_CAPACITY     1024
#define DAY6_ENTRY_ITEMS_CAPACITY 32

structdef(Day7Entry) {
    u64     total;
    Vec_u16 items;
    u32     permutations;
};
DefArrayTypes(Day7Entry);

internal bool day7_eval_permutation(Day7Entry* entry, u32 perm) {
    u64 number = entry->items.items[0];
    for (u32 i = 1; i < entry->items.count; ++i) {
        if (perm & 1) {
            number += entry->items.items[i];
        } else {
            number *= entry->items.items[i];
        }
        perm >>= 1;
    }
    return number == entry->total;
}

internal DayResult day7(Arena* arena, Str input) {
    i64 part1 = 0;
    i64 part2 = 0;

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

        entry.permutations = 1 << (entry.items.count - 1);

        *VecPush(entries) = entry;
    }

    for (u32 i = 0; i < entries.count; ++i) {
        Day7Entry* entry = &entries.items[i];

        for (u32 j = 0; j < entry->permutations; ++j) {
            if (day7_eval_permutation(entry, j)) {
                part1 += entry->total;
                break;
            }
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}