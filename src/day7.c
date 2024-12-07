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

structdef(Day7Perm) {
    u64    error;
    Day7Op ops[DAY6_ENTRY_ITEMS_CAPACITY];
};
DefArrayTypes(Day7Perm);

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

internal u64 day7_eval_perm(Day7Entry* entry, Day7Perm* perm) {
    u64 number = entry->items.items[0];
    for (u32 i = 1; i < entry->items.count; ++i) {
        number = day7_op_exec(perm->ops[i - 1], number, entry->items.items[i]);
    }
    return number;
}

internal u64 day7_count_matches(Arena* arena, Slice_Day7Entry entries, u32 base) {
    u64          result     = 0;
    ArenaMark    mark       = arena_mark(arena);
    Vec_Day7Perm perm_stack = VecAlloc(Day7Perm, arena, Kb(32));

    for (u32 i = 0; i < entries.count; ++i) {
        Dbg(i);
        perm_stack.count = 0;

        Day7Entry* entry = &entries.items[i];
        Day7Perm   perm0 = (Day7Perm){0};
        u64        test  = day7_eval_perm(entry, &perm0);
        i64        delta = (i64)test - (i64)entry->total;
        perm0.error      = (u64)Abs(delta);

        if (test == entry->total) {
            result += entry->total;
            Dbg("success");
            continue;
        }

        VecMinHeapPush(Day7Perm, perm_stack, .error, perm0);

        while (perm_stack.count > 0) {
            Day7Perm perm;
            VecMinHeapPopInto(Day7Perm, perm_stack, .error, perm);

            for (u32 j = 0; j < entry->items.count - 1; ++j) {
                if (perm.ops[j] == base - 1) continue;

                Day7Perm copy = perm;
                copy.ops[j]++;
                test = day7_eval_perm(entry, &copy);

                if (test == entry->total) {
                    result += entry->total;
                    Dbg("success");
                    goto next;
                } else if (test < entry->total) {
                    i64 delta  = (i64)test - (i64)entry->total;
                    copy.error = (u64)Abs(delta);
                    VecMinHeapPush(Day7Perm, perm_stack, .error, copy);
                }
            }
        }

        Dbg("fail");

    next: {}
    }

    arena_restore(arena, mark);
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

    i64 part1 = 0;  // day7_count_matches(arena, entries.slice, 2);
    i64 part2 = day7_count_matches(arena, entries.slice, 3);
    // i64 part2 = 0;

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}