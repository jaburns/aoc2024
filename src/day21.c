#include "main.h"

#define DAY21_BAG_CAPACITY        32
#define DAY21_EXPANSIONS_CAPACITY 64
#define DAY21_ELEM_VEC_CAPACITY   64

enumdef(Day21ElemTag, u8){
    Day21ElemTag_STRING,
    Day21ElemTag_ELEM_SLICE,
    Day21ElemTag_UNDECIDED,
};

typedef struct Day21Elem Day21Elem;
DefArrayTypes(Day21Elem);

typedef struct Day21UndecidedElem Day21UndecidedElem;

struct Day21Elem {
    Day21ElemTag tag;
    union {
        Str32               as_string;
        Slice_Day21Elem     as_elem_slice;
        Day21UndecidedElem* as_undecided;
    };
};

struct Day21UndecidedElem {
    u64       len;
    Day21Elem left;
    Day21Elem right;
};

typedef HashArray Day21Bag;         // Str32 -> u64
typedef HashArray Day21Expansions;  // Str32 -> Slice_Str32

structdef(Day21Parts) {
    u64 part1;
    u64 part2;
};

internal ivec2 day21_get_arrow_coords(char a) {
    switch (a) {
        case '^':
            return ivec2(1, 0);
        case 'A':
            return ivec2(2, 0);
        case '<':
            return ivec2(0, 1);
        case 'v':
            return ivec2(1, 1);
        case '>':
            return ivec2(2, 1);
    }
    AssertUnreachable();
}

internal ivec2 day21_get_numeric_coords(char a) {
    switch (a) {
        case '7':
            return ivec2(0, 0);
        case '8':
            return ivec2(1, 0);
        case '9':
            return ivec2(2, 0);
        case '4':
            return ivec2(0, 1);
        case '5':
            return ivec2(1, 1);
        case '6':
            return ivec2(2, 1);
        case '1':
            return ivec2(0, 2);
        case '2':
            return ivec2(1, 2);
        case '3':
            return ivec2(2, 2);
        case '0':
            return ivec2(1, 3);
        case 'A':
            return ivec2(2, 3);
    }
    AssertUnreachable();
}

internal Day21Elem day21_explode_arrow_pair(Arena* arena, char a, char b) {
    Day21Elem ret = {0};

    ivec2 va        = day21_get_arrow_coords(a);
    ivec2 vb        = day21_get_arrow_coords(b);
    ivec2 delta     = ivec2_sub(vb, va);
    ivec2 delta_abs = ivec2_abs(delta);

    char v_arrow = delta.y >= 0 ? 'v' : '^';
    char h_arrow = delta.x >= 0 ? '>' : '<';

    if (delta.x == 0) {
        str32_push_repeat(&ret.as_string, v_arrow, delta_abs.y);
        str32_push(&ret.as_string, 'A');
        goto end;
    }
    if (delta.y == 0) {
        str32_push_repeat(&ret.as_string, h_arrow, delta_abs.x);
        str32_push(&ret.as_string, 'A');
        goto end;
    }
    if ((a == 'A' || a == '^') && (b == '<')) {
        str32_push_repeat(&ret.as_string, v_arrow, delta_abs.y);
        str32_push_repeat(&ret.as_string, h_arrow, delta_abs.x);
        str32_push(&ret.as_string, 'A');
        goto end;
    }
    if ((b == 'A' || b == '^') && (a == '<')) {
        str32_push_repeat(&ret.as_string, h_arrow, delta_abs.x);
        str32_push_repeat(&ret.as_string, v_arrow, delta_abs.y);
        str32_push(&ret.as_string, 'A');
        goto end;
    }

    ret.tag                  = Day21ElemTag_UNDECIDED;
    Day21UndecidedElem* elem = arena_alloc(arena, sizeof(Day21UndecidedElem));
    ret.as_undecided         = elem;

    elem->len = delta_abs.x + delta_abs.y + 1;

    str32_push_repeat(&elem->left.as_string, v_arrow, delta_abs.y);
    str32_push_repeat(&elem->left.as_string, h_arrow, delta_abs.x);
    str32_push(&elem->left.as_string, 'A');

    str32_push_repeat(&elem->right.as_string, h_arrow, delta_abs.x);
    str32_push_repeat(&elem->right.as_string, v_arrow, delta_abs.y);
    str32_push(&elem->right.as_string, 'A');

end:
    return ret;
}

internal Day21Elem day21_explode_number_pair(Arena* arena, char a, char b) {
    Day21Elem ret = {0};

    ivec2 va        = day21_get_numeric_coords(a);
    ivec2 vb        = day21_get_numeric_coords(b);
    ivec2 delta     = ivec2_sub(vb, va);
    ivec2 delta_abs = ivec2_abs(delta);

    char v_arrow = delta.y >= 0 ? 'v' : '^';
    char h_arrow = delta.x >= 0 ? '>' : '<';

    if (delta.x == 0) {
        str32_push_repeat(&ret.as_string, v_arrow, delta_abs.y);
        str32_push(&ret.as_string, 'A');
        goto end;
    }
    if (delta.y == 0) {
        str32_push_repeat(&ret.as_string, h_arrow, delta_abs.x);
        str32_push(&ret.as_string, 'A');
        goto end;
    }
    if ((a == '0' || a == 'A') && (b == '1' || b == '4' || b == '7')) {
        str32_push_repeat(&ret.as_string, v_arrow, delta_abs.y);
        str32_push_repeat(&ret.as_string, h_arrow, delta_abs.x);
        str32_push(&ret.as_string, 'A');
        goto end;
    }
    if ((b == '0' || b == 'A') && (a == '1' || a == '4' || a == '7')) {
        str32_push_repeat(&ret.as_string, h_arrow, delta_abs.x);
        str32_push_repeat(&ret.as_string, v_arrow, delta_abs.y);
        str32_push(&ret.as_string, 'A');
        goto end;
    }

    ret.tag                  = Day21ElemTag_UNDECIDED;
    Day21UndecidedElem* elem = arena_alloc(arena, sizeof(Day21UndecidedElem));
    ret.as_undecided         = elem;

    elem->len = delta_abs.x + delta_abs.y + 1;

    str32_push_repeat(&elem->left.as_string, v_arrow, delta_abs.y);
    str32_push_repeat(&elem->left.as_string, h_arrow, delta_abs.x);
    str32_push(&elem->left.as_string, 'A');

    str32_push_repeat(&elem->right.as_string, h_arrow, delta_abs.x);
    str32_push_repeat(&elem->right.as_string, v_arrow, delta_abs.y);
    str32_push(&elem->right.as_string, 'A');

end:
    return ret;
}

internal u64 day21_count_length(Day21Elem* elem) {
    switch (elem->tag) {
        case Day21ElemTag_ELEM_SLICE: {
            u64 count = 0;
            for (u32 i = 0; i < elem->as_elem_slice.count; ++i) {
                count += day21_count_length(&elem->as_elem_slice.items[i]);
            }
            return count;
        }
        case Day21ElemTag_STRING: {
            return elem->as_string.count;
        }
        case Day21ElemTag_UNDECIDED: {
            return elem->as_undecided->len;
        }
    }
    AssertUnreachable();
}

internal Day21Elem day21_explode_arrows(Arena* arena, Day21Elem elem) {
    switch (elem.tag) {
        case Day21ElemTag_ELEM_SLICE: {
            for (u32 i = 0; i < elem.as_elem_slice.count; ++i) {
                elem.as_elem_slice.items[i] = day21_explode_arrows(arena, elem.as_elem_slice.items[i]);
            }
            break;
        }
        case Day21ElemTag_STRING: {
            Vec_Day21Elem sections = VecAlloc(Day21Elem, arena, DAY21_ELEM_VEC_CAPACITY);
            for (u32 i = 0; i < elem.as_string.count; ++i) {
                char      first    = i == 0 ? 'A' : elem.as_string.items[i - 1];
                Day21Elem exploded = day21_explode_arrow_pair(arena, first, elem.as_string.items[i]);
                *VecPush(sections) = exploded;
            }
            elem.tag           = Day21ElemTag_ELEM_SLICE;
            elem.as_elem_slice = sections.slice;
            break;
        }
        case Day21ElemTag_UNDECIDED: {
            Day21Elem left  = day21_explode_arrows(arena, elem.as_undecided->left);
            Day21Elem right = day21_explode_arrows(arena, elem.as_undecided->right);

            u64 left_len  = day21_count_length(&left);
            u64 right_len = day21_count_length(&right);

            if (left_len < right_len) return left;
            if (left_len > right_len) return right;

            elem.as_undecided        = arena_alloc(arena, sizeof(Day21UndecidedElem));
            elem.as_undecided->len   = left_len;
            elem.as_undecided->left  = left;
            elem.as_undecided->right = right;
            break;
        }
    }
    return elem;
}

internal void day21_flatten_into(Arena* arena, Vec_Day21Elem* out, Day21Elem elem) {
    switch (elem.tag) {
        case Day21ElemTag_ELEM_SLICE: {
            for (u32 i = 0; i < elem.as_elem_slice.count; ++i) {
                day21_flatten_into(arena, out, elem.as_elem_slice.items[i]);
            }
            break;
        }
        case Day21ElemTag_STRING: {
            *VecPush(*out) = elem;
            break;
        }
        case Day21ElemTag_UNDECIDED: {
            Vec_Day21Elem flat_left = VecAlloc(Day21Elem, arena, DAY21_ELEM_VEC_CAPACITY);
            day21_flatten_into(arena, &flat_left, elem.as_undecided->left);

            Vec_Day21Elem flat_right = VecAlloc(Day21Elem, arena, DAY21_ELEM_VEC_CAPACITY);
            day21_flatten_into(arena, &flat_right, elem.as_undecided->right);

            u64 len = elem.as_undecided->len;

            elem.as_undecided                      = arena_alloc(arena, sizeof(Day21UndecidedElem));
            elem.as_undecided->len                 = len;
            elem.as_undecided->left.tag            = Day21ElemTag_ELEM_SLICE;
            elem.as_undecided->left.as_elem_slice  = flat_left.slice;
            elem.as_undecided->len                 = len;
            elem.as_undecided->right.tag           = Day21ElemTag_ELEM_SLICE;
            elem.as_undecided->right.as_elem_slice = flat_right.slice;

            *VecPush(*out) = elem;
            break;
        }
    }
}

internal Day21Elem day21_pick_best_branches(Arena* arena, Day21Elem elem) {
    DebugAssert(elem.tag != Day21ElemTag_ELEM_SLICE);
    if (elem.tag == Day21ElemTag_STRING) return elem;

    Day21Elem left     = elem.as_undecided->left;
    Day21Elem right    = elem.as_undecided->right;
    Day21Elem og_left  = left;
    Day21Elem og_right = right;

    for (;;) {
        Vec_Day21Elem flat_left = VecAlloc(Day21Elem, arena, DAY21_ELEM_VEC_CAPACITY);
        day21_flatten_into(arena, &flat_left, day21_explode_arrows(arena, left));
        left.tag           = Day21ElemTag_ELEM_SLICE;
        left.as_elem_slice = flat_left.slice;

        Vec_Day21Elem flat_right = VecAlloc(Day21Elem, arena, DAY21_ELEM_VEC_CAPACITY);
        day21_flatten_into(arena, &flat_right, day21_explode_arrows(arena, right));
        right.tag           = Day21ElemTag_ELEM_SLICE;
        right.as_elem_slice = flat_right.slice;

        u64 left_len  = day21_count_length(&left);
        u64 right_len = day21_count_length(&right);

        if (left_len < right_len) return og_left;
        if (left_len > right_len) return og_right;
    }

    AssertUnreachable();
}

internal Slice_Str32 day21_get_arrow_expansion(Arena* arena, Day21Expansions* expansions, Str32 str) {
    Slice_Str32* entry = hasharray_entry(expansions, &str);

    if (!entry->count) {
        Day21Elem elem     = {0};
        elem.as_string     = str;
        Day21Elem exploded = day21_explode_arrows(arena, elem);
        DebugAssert(exploded.tag == Day21ElemTag_ELEM_SLICE);

        *entry = SliceAlloc(Str32, arena, exploded.as_elem_slice.count);
        for (u32 i = 0; i < exploded.as_elem_slice.count; ++i) {
            Day21Elem flattened = day21_pick_best_branches(arena, exploded.as_elem_slice.items[i]);
            DebugAssert(flattened.tag == Day21ElemTag_STRING);
            entry->items[i] = flattened.as_string;
        }
    }

    return *entry;
}

internal void day21_add_to_bag(Day21Bag* bag, Str32 str, u64 count) {
    u64* entry  = (u64*)hasharray_entry(bag, &str);
    *entry     += count;
}

internal void day21_step_bag(Arena* arena, Day21Expansions* expansions, Day21Bag* in_bag, Day21Bag* out_bag) {
    foreach (HashArrayIter, it, in_bag) {
        u64         count    = *(u64*)it.value;
        Slice_Str32 expanded = day21_get_arrow_expansion(arena, expansions, *(Str32*)it.key);
        for (u32 i = 0; i < expanded.count; ++i) {
            day21_add_to_bag(out_bag, expanded.items[i], count);
        }
    }
}

internal u64 day21_count_bag(Day21Bag* bag) {
    u64 ret = 0;
    foreach (HashArrayIter, it, bag) {
        u64 count  = *(u64*)it.value;
        u8  len    = ((Str32*)it.key)->count;
        ret       += count * len;
    }
    return ret;
}

internal Day21Parts day21_solve(
    Arena* arena, Day21Expansions* expansions,
    Day21Bag* bag0, Day21Bag* bag1, Str numbers
) {
    hasharray_clear(bag0);

    for (u32 i = 0; i < numbers.count; ++i) {
        char      first    = i == 0 ? 'A' : numbers.items[i - 1];
        Day21Elem exploded = day21_explode_number_pair(arena, first, numbers.items[i]);
        Day21Elem init     = day21_pick_best_branches(arena, exploded);
        DebugAssert(init.tag == Day21ElemTag_STRING);
        day21_add_to_bag(bag0, init.as_string, 1);
    }

    numbers.count = 3;
    u64 mul       = str_parse_u64(numbers, 10);

    Day21Parts ret = {0};

    for (u32 i = 0; i < 25; ++i) {
        hasharray_clear(bag1);
        day21_step_bag(arena, expansions, bag0, bag1);
        Swap(Day21Bag*, bag0, bag1);

        if (i == 1) ret.part1 = day21_count_bag(bag0) * mul;
    }
    ret.part2 = day21_count_bag(bag0) * mul;

    return ret;
}

internal DayResult day21(Arena* arena, Str input) {
    Day21Expansions* expansions = hasharray_alloc_with_cap(arena, sizeof(Str32), sizeof(Slice_Str32), DAY21_EXPANSIONS_CAPACITY);
    Day21Bag*        bag0       = hasharray_alloc_with_cap(arena, sizeof(Str32), sizeof(u64), DAY21_BAG_CAPACITY);
    Day21Bag*        bag1       = hasharray_alloc_with_cap(arena, sizeof(Str32), sizeof(u64), DAY21_BAG_CAPACITY);

    Day21Parts parts[5];
    u32        i = 0;
    foreach (StrSplitIter, it, '\n', input) {
        parts[i++] = day21_solve(arena, expansions, bag0, bag1, it.item);
    }

    u64 part1 = 0;
    u64 part2 = 0;
    for (u32 i = 0; i < 5; ++i) {
        part1 += parts[i].part1;
        part2 += parts[i].part2;
    }

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}