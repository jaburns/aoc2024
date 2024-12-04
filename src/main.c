
#include "main.h"

#include "../jaburns_c/base/inc.c"

#define BIGBOY        0
#define DAY_NUMBER    4
#define INPUT_TYPE    "main"
#define HIDE_SOLUTION 1

#define ALLOCATION_SIZE (1 << 30)

#if DEBUG || BIGBOY
#define ITERATIONS 1
#else
#define ITERATIONS 1000
#endif

#include "day1.c"
#include "day2.c"
#include "day3.c"
#include "day4.c"

#define DayFn(x)  Concatenate(day, x)
#define DayStr(x) Stringify(x)

internal volatile i64 dump;

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
    Arena arena = arena_create(&GLOBAL_ALLOCATOR, ALLOCATION_SIZE);

    Str input = str_read_file(&arena, "inputs/day" DayStr(DAY_NUMBER) "-" INPUT_TYPE ".txt");

    DayResult result;
    u64       total_time = 0;
    for (u32 i = 0; i < ITERATIONS; ++i) {
        ArenaMark mark = arena_mark(&arena);

        u64 start = timing_get_ticks();
        result    = DayFn(DAY_NUMBER)(&arena, input);
        u64 delta = timing_get_ticks() - start;

        if (i >= ITERATIONS / 2) total_time += delta;

        arena_restore(&arena, mark);
    }

    dump = result.parts[0].as_i64;
    dump = result.parts[1].as_i64;

    printf("\n");
#if BIGBOY
    printf("-- DAY " DayStr(DAY_NUMBER) " BIGBOY --\n");
#else
    printf("-- DAY " DayStr(DAY_NUMBER) " --\n");
#endif
    printf("   Time: ");
    print_time(&arena, timing_ticks_to_nanos(total_time) / (Max(1, ITERATIONS / 2)));
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