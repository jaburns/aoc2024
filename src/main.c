#include "main.h"

#include "../jaburns_c/base/inc.c"

#include "day1.c"
#include "day2.c"
#include "day3.c"

#define DAY_NUMBER    3
#define INPUT_TYPE    "main"
#define ITERATIONS    1000
#define HIDE_SOLUTION 0

#define DayFn(x)  Concatenate(day, x)
#define DayStr(x) Stringify(x)

internal void print_result_part(DayResultPart* part) {
    if (part->is_str) {
        printf("%.*s", StrPrintfArgs(part->as_str));
    } else {
        printf("%lli", part->as_i64);
    }
}

internal void print_time(Arena* arena, u64 nanos) {
    char* units = "ns";
    if (nanos > 1000000000) {
        nanos /= 1000000;
        units  = "ms";
    } else if (nanos > 1000000) {
        nanos /= 1000;
        units  = "μs";
    }
    printf("%s %s", u64_print_with_commas(arena, nanos), units);
}

i32 main(int argc, char** argv) {
    scratch_thread_local_create(&GLOBAL_ALLOCATOR);
    timing_global_init();

    ArenaTemp scratch = scratch_acquire(NULL, 0);
    Str       input   = str_read_file(scratch.arena, "inputs/day" DayStr(DAY_NUMBER) "-" INPUT_TYPE ".txt");

    DayResult result;
    u64       total_time = 0;
    for (u32 i = 0; i < ITERATIONS; ++i) {
        ArenaMark mark = arena_mark(scratch.arena);

        u64 start = timing_get_ticks();
        result    = DayFn(DAY_NUMBER)(scratch.arena, input);
        u64 delta = timing_get_ticks() - start;

        if (i >= ITERATIONS / 2) total_time += delta;

        arena_restore(scratch.arena, mark);
    }

    printf("\n");
    printf("-- DAY " DayStr(DAY_NUMBER) " --\n");
    printf("   Time: ");
    print_time(scratch.arena, timing_ticks_to_nanos(total_time) / (ITERATIONS / 2));
    printf("\n");
    printf(" Part 1: ");
#if HIDE_SOLUTION
    printf("[x]");
#else
    print_result_part(&result.parts[0]);
#endif
    printf("\n");
    printf(" Part 2: ");
#if HIDE_SOLUTION
    printf("[x]");
#else
    print_result_part(&result.parts[1]);
#endif
    printf("\n\n");

    return 0;
}