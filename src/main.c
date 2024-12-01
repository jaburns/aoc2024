#include "main.h"

#include "../jaburns_c/base/inc.c"

DayResult day1(Arena* arena, Str input);  // defined in "day1.s"
#include "day2.c"

#define DAY_NUMBER 1
#define INPUT_TYPE "main"
#define ITERATIONS 1

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

        u64 start = timing_get_nanos_since_start();
        result    = DayFn(DAY_NUMBER)(scratch.arena, input);
        u64 delta = timing_get_nanos_since_start() - start;

        total_time += delta;

        arena_restore(scratch.arena, mark);
    }

    printf("\n");
    printf("-- DAY " DayStr(DAY_NUMBER) " --\n");
    printf("   Time: ");
    print_time(scratch.arena, total_time / ITERATIONS);
    printf("\n");
    printf(" Part 1: ");
    print_result_part(&result.parts[0]);
    printf("\n");
    printf(" Part 2: ");
    print_result_part(&result.parts[1]);
    printf("\n\n");

    return 0;
}