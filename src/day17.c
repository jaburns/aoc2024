#include "main.h"

#define DAY17_OUTPUT_MAX_SIZE Kb(1)

#if 0
internal void day17_run_vm(char* program, usize max_ip, u64 a_register, char* output) {
#define Literal(x) (x - '0')
#define Combo(x)   (x <= '3' ? (x - '0') : registers[x - '4'])
    u64 registers[3] = {a_register, 0, 0};
    u64 ip = 0;
    while (ip <= max_ip) {
        char operand = program[2 * ip + 2];
        switch (program[2 * ip]) {
            case '0': {  // adv
                registers[0] >>= Combo(operand);
                break;
            }
            case '1': {  // bxl
                registers[1] ^= Literal(operand);
                break;
            }
            case '2': {  // bst
                registers[1] = Combo(operand) & 0b111;
                break;
            }
            case '3': {  // jnz
                if (registers[0]) {
                    ip = Literal(operand);
                    continue;
                }
                break;
            }
            case '4': {  // bxc
                registers[1] ^= registers[2];
                break;
            }
            case '5': {  // out
                output += sprintf(output, "%llu,", (u64)(Combo(operand) & 0b111));
                break;
            }
            case '6': {  // bdv
                registers[1] = registers[0] >> Combo(operand);
                break;
            }
            case '7': {  // cdv
                registers[2] = registers[0] >> Combo(operand);
                break;
            }
        }
        ip += 2;
    }
    *--output = 0;
#undef Literal
#undef Combo
}
#endif

internal u8 day17_cypher(u64 a) {
    u64 b, c;
    b = a & 0b111;
    b = b ^ 1;
    c = (a >> b) & 0b111;
    b = b ^ 5;
    b = b ^ c;
    return b;
}

internal void day17_run_reversed(u64 a_register, char* output) {
    u64 a = a_register;

    while (a > 0) {
        u8 ch   = day17_cypher(a);
        output += sprintf(output, "%u,", ch);
        a       = a >> 3;
    }

    *--output = 0;
}

internal u64 day17_append_digit(u64 total, u8 digit, u8 match_idx) {
    total <<= 3;
    for (u32 i = 0; i < 8; ++i) {
        if (day17_cypher(total | i) == digit) {
            if (match_idx == 0) return total | i;
            match_idx--;
        }
    }
    return 0;
}

internal u8 day17_count_matches(u64 total, u8 digit) {
    u8 count   = 0;
    total    <<= 3;
    for (u32 i = 0; i < 8; ++i) {
        if (day17_cypher(total | i) == digit) count++;
    }
    return count;
}

internal u64 day17_try_encode(u64 total, usize idx, Slice_u8 target) {
    if (idx == target.count) return total;

    u8 matches = day17_count_matches(total, target.items[idx]);

    for (u8 m = 0; m < matches; ++m) {
        u64 new_total = day17_append_digit(total, target.items[idx], m);
        if (new_total == 0) continue;

        u64 result = day17_try_encode(new_total, idx + 1, target);
        if (result != 0) return result;
    }

    return 0;
}

internal DayResult day17(Arena* arena, Str input) {
    StrSplitIter lines_iter       = StrSplitIter_new('\n', input);
    u64          part1_register_a = str_parse_u64(str_trim(str_after_last_index(':', lines_iter.item)), 10);
    StrSplitIter_next(&lines_iter);
    StrSplitIter_next(&lines_iter);
    StrSplitIter_next(&lines_iter);
    StrSplitIter_next(&lines_iter);

    Str    program_str = str_trim(str_after_last_index(':', lines_iter.item));
    Vec_u8 program     = VecAlloc(u8, arena, 32);
    foreach (StrSplitIter, elems_it, ',', program_str) {
        *VecPush(program) = (u8)str_parse_u32(elems_it.item, 10);
    }
    ArrayReverse(u8, program.items, program.count);

    char* output = arena_alloc(arena, DAY17_OUTPUT_MAX_SIZE);
    day17_run_reversed(part1_register_a, output);
    Str part1 = str_copy(arena, str_from_cstr(output));
    u64 part2 = day17_try_encode(0, 0, program.slice);

    DayResult result            = {0};
    result.parts[0].is_str      = true;
    result.parts[0].as_str      = part1;
    result.parts[1].as_i64      = part2;
    result.has_manual_component = true;
    return result;
}