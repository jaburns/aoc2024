// -------------------------------
#define DEBUG         0
#define BIGBOY_INPUTS 0
#define TEST_INPUTS   0
#define DAY_NUMBER    17
#define HIDE_SOLUTION 0
// -------------------------------

#include "main.h"

#include "../jaburns_c/base/inc.c"

#if BIGBOY_INPUTS
#define ALLOCATION_SIZE Gb(8)
#define INPUT_TYPE      "bigboy"
#elif TEST_INPUTS
#define ALLOCATION_SIZE Mb(128)
#define INPUT_TYPE      "test"
#else
#define ALLOCATION_SIZE Mb(128)
#define INPUT_TYPE      "main"
#endif

#if DEBUG || BIGBOY_INPUTS
#define ITERATIONS 1
#else
#define ITERATIONS 10
#endif

#include "day1.c"
#include "day2.c"
#include "day3.c"
#include "day4.c"
#include "day5.c"
#include "day6.c"
#include "day7.c"
#include "day8.c"
#include "day9.c"
#include "day10.c"
#include "day11.c"
#include "day12.c"
#include "day13.c"
#include "day14.c"
#include "day15.c"
#include "day16.c"
#include "day17.c"
#include "day18.c"
#include "day19.c"
#include "day20.c"
#include "day21.c"
#include "day22.c"
#include "day23.c"
#include "day24.c"
#include "day25.c"

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
    if (nanos > 100000000) {
        nanos /= 1000000;
        units  = "ms";
    } else if (nanos >= 100000) {
        nanos /= 1000;
        units  = "μs";
    }
    printf("%s %s", u64_print_with_commas(arena, nanos), units);
}

void run_individual() {
    scratch_thread_local_create(&GLOBAL_ALLOCATOR, ALLOCATION_SIZE);
    ArenaTemp scratch = scratch_acquire(NULL, 0);
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

        if (i == 0) {
            if (result.parts[0].is_str) result.parts[0].as_str = str_copy(scratch.arena, result.parts[0].as_str);
            if (result.parts[1].is_str) result.parts[1].as_str = str_copy(scratch.arena, result.parts[1].as_str);
        }

        if (i >= ITERATIONS / 2) total_time += delta;

        arena_restore(&arena, mark);
    }

    printf("\n");
#if BIGBOY_INPUTS
    printf("-- DAY " DayStr(DAY_NUMBER) " BIGBOY --\n");
#else
    printf("-- DAY " DayStr(DAY_NUMBER) " --\n");
#endif
    printf("   Time: ");
    print_time(&arena, timing_ticks_to_nanos(total_time) / (Max(1, ITERATIONS / 2)));
    if (result.has_manual_component) printf(" + manual labor");
    printf("\n");
    printf(" Part 1: ");
#if HIDE_SOLUTION
    printf(result.parts[0].as_i64 ? "[x]" : "[ ]");
#else
    print_result_part(&result.parts[0]);
#endif
    printf("\n");
    printf(" Part 2: ");
#if HIDE_SOLUTION
    printf(result.parts[1].as_i64 ? "[x]" : "[ ]");
#else
    print_result_part(&result.parts[1]);
#endif
    printf("\n\n");
}

volatile u64 optimizer_sink;

void run_summary(void) {
    scratch_thread_local_create(&GLOBAL_ALLOCATOR, ALLOCATION_SIZE);
    ArenaTemp scratch = scratch_acquire(NULL, 0);
    timing_global_init();
    Arena arena = arena_create(&GLOBAL_ALLOCATOR, ALLOCATION_SIZE);

    u64 total_total_time = 0;

#define RunDay(day_number)                                                                                            \
    do {                                                                                                              \
        Str       input = str_read_file(&arena, "inputs/day" DayStr(day_number) "-" INPUT_TYPE ".txt");               \
        DayResult result;                                                                                             \
        u64       total_time = 0;                                                                                     \
        for (u32 i = 0; i < ITERATIONS; ++i) {                                                                        \
            ArenaMark mark  = arena_mark(&arena);                                                                     \
            u64       start = timing_get_ticks();                                                                     \
            result          = DayFn(day_number)(&arena, input);                                                       \
            u64 delta       = timing_get_ticks() - start;                                                             \
            if (i == 0) {                                                                                             \
                if (result.parts[0].is_str) result.parts[0].as_str = str_copy(scratch.arena, result.parts[0].as_str); \
                if (result.parts[1].is_str) result.parts[1].as_str = str_copy(scratch.arena, result.parts[1].as_str); \
            }                                                                                                         \
            if (i >= ITERATIONS / 2) total_time += delta;                                                             \
            arena_restore(&arena, mark);                                                                              \
        }                                                                                                             \
        optimizer_sink    = u64_wrapped_add(optimizer_sink, result.parts[0].as_i64);                                  \
        optimizer_sink    = u64_wrapped_add(optimizer_sink, result.parts[1].as_i64);                                  \
        total_total_time += total_time;                                                                               \
        printf("  Day %02d : ", day_number);                                                                          \
        print_time(&arena, timing_ticks_to_nanos(total_time) / (Max(1, ITERATIONS / 2)));                             \
        if (result.has_manual_component) printf(" + manual labor");                                                   \
        printf("\n");                                                                                                 \
    } while (0)

    printf("\n");
    RunDay(1);
    RunDay(2);
    RunDay(3);
    RunDay(4);
    RunDay(5);
    RunDay(6);
    RunDay(7);
    RunDay(8);
    RunDay(9);
    RunDay(10);
    RunDay(11);
    RunDay(12);
    RunDay(13);
    RunDay(14);
    RunDay(15);
    RunDay(16);
    RunDay(17);
    RunDay(18);
    RunDay(19);
    RunDay(20);
    RunDay(21);
    RunDay(22);
    RunDay(23);
    RunDay(24);
    RunDay(25);
    printf("   Total : ");
    print_time(&arena, timing_ticks_to_nanos(total_total_time));
    printf("\n\n");

#undef RunDay
}

i32 main(int argc, char** argv) {
    run_summary();
    return 0;
}