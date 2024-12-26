#include "main.h"

#if TEST_INPUTS
#define DAY24_BIT_COUNT 13
#else
#define DAY24_BIT_COUNT 46
#endif

enumdef(Day24Op, u8){
    Day24Op_CONST,
    Day24Op_AND,
    Day24Op_OR,
    Day24Op_XOR,
};

structdef(Day24WireName) {
    char str[3];
};

structdef(Day24Rule) {
    Day24WireName lhs, rhs;
    bool          const_value;
    Day24Op       op;
};

internal bool day24_eval_wire(HashArray* rules, Day24WireName wire) {
    Day24Rule* rule = hasharray_get(rules, &wire);

    switch (rule->op) {
        case Day24Op_CONST:
            return rule->const_value;
            break;
        case Day24Op_AND:
            return day24_eval_wire(rules, rule->lhs) && day24_eval_wire(rules, rule->rhs);
            break;
        case Day24Op_OR:
            return day24_eval_wire(rules, rule->lhs) || day24_eval_wire(rules, rule->rhs);
            break;
        case Day24Op_XOR:
            return day24_eval_wire(rules, rule->lhs) ^ day24_eval_wire(rules, rule->rhs);
            break;
    }
    AssertUnreachable();
}

internal DayResult day24(Arena* arena, Str input) {
    HashArray* rules = hasharray_alloc_with_elems(arena, sizeof(Day24WireName), sizeof(Day24Rule), 512);

    bool init_mode = true;
    foreach (StrSplitIter, lines_it, '\n', input) {
        if (init_mode) {
            if (lines_it.item.count == 0) {
                init_mode = false;
                continue;
            }

            Day24WireName name;
            memcpy(name.str, lines_it.item.items, 3);

            *(Day24Rule*)hasharray_insert(rules, &name) = (Day24Rule){
                .const_value = lines_it.item.items[5] == '1',
                .op          = Day24Op_CONST,
            };
        } else {
            Str dest_str = str_trim(str_substr(lines_it.item, 14, 4));
            Str rhs_str  = str_trim(str_substr(lines_it.item, 7, 4));
            Str lhs_str  = str_substr(lines_it.item, 0, 3);

            Day24WireName dest, rhs, lhs;
            memcpy(dest.str, dest_str.items, 3);
            memcpy(rhs.str, rhs_str.items, 3);
            memcpy(lhs.str, lhs_str.items, 3);

            Day24Op op;
            switch (lines_it.item.items[4]) {
                case 'A':
                    op = Day24Op_AND;
                    break;
                case 'O':
                    op = Day24Op_OR;
                    break;
                case 'X':
                    op = Day24Op_XOR;
                    break;
                default:
                    AssertUnreachable();
            }

            *(Day24Rule*)hasharray_insert(rules, &dest) = (Day24Rule){
                .lhs = lhs,
                .rhs = rhs,
                .op  = op,
            };
        }
    }

    u64 part1 = 0;
    for (u32 i = 0; i < DAY24_BIT_COUNT; ++i) {
        Day24WireName bit_name  = {0};
        bit_name.str[0]         = 'z';
        bit_name.str[1]         = (i / 10) + '0';
        bit_name.str[2]         = (i % 10) + '0';
        part1                  |= (u64)day24_eval_wire(rules, bit_name) << i;
    }

    // print out which individual adders are broken, so we can look at them in the graph tool
#if 0
    for (u64 test_bit = 0; test_bit < DAY24_BIT_COUNT - 1; ++test_bit) {
        for (u32 i = 0; i < DAY24_BIT_COUNT - 1; ++i) {
            Day24WireName name = {0};
            name.str[1]        = (i / 10) + '0';
            name.str[2]        = (i % 10) + '0';

            name.str[0]       = 'x';
            Day24Rule* rule   = hasharray_get(rules, &name);
            rule->const_value = i == test_bit;

            name.str[0]       = 'y';
            rule              = hasharray_get(rules, &name);
            rule->const_value = i == test_bit;
        }

        u64 part2 = 0;
        for (u32 i = 0; i < DAY24_BIT_COUNT; ++i) {
            Day24WireName bit_name  = {0};
            bit_name.str[0]         = 'z';
            bit_name.str[1]         = (i / 10) + '0';
            bit_name.str[2]         = (i % 10) + '0';
            part2                  |= (u64)day24_eval_wire(rules, bit_name) << i;
        }

        Dbg(test_bit);
        if ((1LLU << (test_bit + 1LLU)) == part2) {
            Dbg("Good");
        } else {
            Dbg("Bad");
        }
    }
#endif

    DayResult result            = {0};
    result.parts[0].as_i64      = part1;
    result.parts[1].is_str      = true;
    result.parts[1].as_str      = Str("fbq,pbv,qff,qnw,qqp,z16,z23,z36");
    result.has_manual_component = true;
    return result;
}