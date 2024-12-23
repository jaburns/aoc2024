#include "main.h"

structdef(Day23Loop) {
    u32 elems[3];
};
DefArrayTypes(Day23Loop);

internal u32 day23_chars_to_id(char a, char b) {
    return (u32)(a - 'a') * 26 + (u32)(b - 'a');
}

internal void day23_count_loops(HashArray* connections, u32 this_id, u32 target_id, u32 depth, Day23Loop cur_loop, Vec_Day23Loop* out) {
    Vec_u32* entry = hasharray_get(connections, &this_id);

    if (depth == 0) {
        for (u32 i = 0; i < entry->count; ++i) {
            if (entry->items[i] == target_id) {
                cur_loop.elems[0] = target_id;
                *VecPush(*out)    = cur_loop;
                return;
            }
        }
        return;
    }

    for (u32 i = 0; i < entry->count; ++i) {
        cur_loop.elems[depth] = entry->items[i];
        day23_count_loops(connections, entry->items[i], target_id, depth - 1, cur_loop, out);
    }
}

internal DayResult day23(Arena* arena, Str input) {
    HashArray* connections = hasharray_alloc_with_cap(arena, sizeof(u32), sizeof(Vec_u32), 1024);

    foreach (StrSplitIter, lines_it, '\n', input) {
        u32 id0 = day23_chars_to_id(lines_it.item.items[0], lines_it.item.items[1]);
        u32 id1 = day23_chars_to_id(lines_it.item.items[3], lines_it.item.items[4]);

        Vec_u32* entry = hasharray_entry(connections, &id0);
        if (!entry->capacity) *entry = VecAlloc(u32, arena, 64);
        *VecPush(*entry) = id1;

        entry = hasharray_entry(connections, &id1);
        if (!entry->capacity) *entry = VecAlloc(u32, arena, 64);
        *VecPush(*entry) = id0;
    }

    u32 min_id = day23_chars_to_id('t', 'a');
    u32 max_id = day23_chars_to_id('t', 'z');

    HashArray* seen  = hasharray_alloc_with_cap(arena, sizeof(Day23Loop), sizeof(bool), 4096);
    u64        part1 = 0;

    for (u32 id = min_id; id <= max_id; ++id) {
        Vec_Day23Loop found = VecAlloc(Day23Loop, arena, 1024);
        day23_count_loops(connections, id, id, 2, (Day23Loop){0}, &found);

        for (u32 i = 0; i < found.count; ++i) {
            ArrayQuickSort(found.items[i].elems, 3, sort_fn_i32_asc);
            bool* entry = hasharray_entry(seen, &found.items[i]);
            if (!*entry) part1++;
            *entry = true;
        }
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    return result;
}