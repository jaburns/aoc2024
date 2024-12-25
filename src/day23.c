#include "main.h"

structdef(Day23Pair) {
    u32 a, b;
};
DefArrayTypes(Day23Pair);

structdef(Day23Loop) {
    u32 elems[3];
};
DefArrayTypes(Day23Loop);

typedef StaticVec(u32, 16) Day23Connections;

internal u32 day23_chars_to_id(char a, char b) {
    return (u32)(a - 'a') * 26 + (u32)(b - 'a');
}

internal bool day23_connected(HashArray* connections, u32 a, u32 b) {
    Day23Connections* entry = hasharray_get(connections, &a);
    for (u32 i = 0; i < entry->count; ++i) {
        if (entry->items[i] == b) return true;
    }
    return false;
}

internal Day23Connections day23_find_best_cluster(HashArray* connections, HashArray* clusters_seen, Day23Connections cluster) {
    Day23Connections* neighbors    = hasharray_get(connections, &cluster.items[0]);
    Day23Connections  best_cluster = cluster;

    for (u32 i = 0; i < neighbors->count; ++i) {
        u32 neighbor = neighbors->items[i];

        for (u32 j = 1; j < cluster.count; ++j) {
            if (neighbor == cluster.items[j]) goto outer_continue;
        }
        for (u32 j = 1; j < cluster.count; ++j) {
            if (!day23_connected(connections, neighbor, cluster.items[j])) goto outer_continue;
        }

        Day23Connections bigger_start = cluster;
        *StaticVecPush(bigger_start)  = neighbor;

        ArrayQuickSort(bigger_start.items, bigger_start.count, sort_fn_i32_asc);
        bool* seen_entry = hasharray_entry(clusters_seen, &bigger_start);
        if (!*seen_entry) {
            *seen_entry                  = true;
            Day23Connections new_cluster = day23_find_best_cluster(connections, clusters_seen, bigger_start);
            if (new_cluster.count > best_cluster.count) {
                best_cluster = new_cluster;
            }
        }

    outer_continue: {}
    }

    return best_cluster;
}

internal void day23_count_loops(HashArray* connections, u32 this_id, u32 target_id, u32 depth, Day23Loop cur_loop, Vec_Day23Loop* out) {
    Day23Connections* entry = hasharray_get(connections, &this_id);

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
    HashArray*    connections = hasharray_alloc_with_cap(arena, sizeof(u32), sizeof(Day23Connections), 1024);
    HashArray*    loops_seen  = hasharray_alloc_with_cap(arena, sizeof(Day23Loop), sizeof(bool), Kb(16));
    Vec_Day23Pair pairs       = VecAlloc(Day23Pair, arena, 4096);

    foreach (StrSplitIter, lines_it, '\n', input) {
        u32 id0 = day23_chars_to_id(lines_it.item.items[0], lines_it.item.items[1]);
        u32 id1 = day23_chars_to_id(lines_it.item.items[3], lines_it.item.items[4]);

        Day23Connections* entry = hasharray_entry(connections, &id0);
        *StaticVecPush(*entry)  = id1;
        entry                   = hasharray_entry(connections, &id1);
        *StaticVecPush(*entry)  = id0;

        *VecPush(pairs) = (Day23Pair){id0, id1};
    }

    u32 part1 = 0;
    {
        Vec_Day23Loop found = VecAlloc(Day23Loop, arena, 256);

        u32 min_id = day23_chars_to_id('a', 'a');
        u32 max_id = day23_chars_to_id('z', 'z');
        for (u32 id = min_id; id <= max_id; ++id) {
            day23_count_loops(connections, id, id, 2, (Day23Loop){0}, &found);

            for (u32 i = 0; i < found.count; ++i) {
                ArrayQuickSort(found.items[i].elems, 3, sort_fn_i32_asc);
                bool* entry = hasharray_entry(loops_seen, &found.items[i]);
                if (!*entry) part1++;
                *entry = true;
            }

            found.count = 0;
        }
    }

    Vec_char part2 = VecAlloc(char, arena, 256);
    {
        HashArray* clusters_seen = hasharray_alloc_with_cap(arena, 3 * sizeof(u32), sizeof(bool), Kb(4));

        Day23Connections best_cluster = {0};
        for (u32 i = 0; i < pairs.count; ++i) {
            Day23Connections start = {0};
            *StaticVecPush(start)  = pairs.items[i].a;
            *StaticVecPush(start)  = pairs.items[i].b;

            Day23Connections cluster = day23_find_best_cluster(connections, clusters_seen, start);
            if (cluster.count > best_cluster.count) {
                best_cluster = cluster;
            }
        }

        ArrayQuickSort(best_cluster.items, best_cluster.count, sort_fn_i32_asc);
        for (u32 i = 0; i < best_cluster.count; ++i) {
            u32  id         = best_cluster.items[i];
            char lo         = (char)(id % 26) + 'a';
            char hi         = (char)(id / 26) + 'a';
            *VecPush(part2) = hi;
            *VecPush(part2) = lo;
            *VecPush(part2) = ',';
        }
        part2.count--;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].is_str = true;
    result.parts[1].as_str = part2.slice;
    return result;
}