#include "../jaburns_c/base/inc.c"
#include "day1.c"

readonly_global DayFn days[2] = {
    NULL,
    day1
};

internal void print_result_part(DayResultPart* part) {
    if (part->is_str) {
        printf("%.*s", STR_PRINTF_ARGS(part->as_str));
    } else {
        printf("%llu", part->as_u64);
    }
}

internal void run_day(u32 day) {
    u64 start = timing_get_nanos_since_start();
    DayResult result = days[day]();
    u64 delta = timing_get_nanos_since_start() - start;

    ArenaTemp scratch = scratch_acquire(NULL, 0);
    printf(
        "Day %u:  %s ns  -  ",
        day,
        u64_print_with_commas(scratch.arena, delta)
    );
    print_result_part(&result.parts[0]);
    printf("  ");
    print_result_part(&result.parts[1]);
    printf("\n");
}

i32 main(int argc, char** argv) {
    scratch_thread_local_create(&GLOBAL_ALLOCATOR);
    timing_global_init();

    run_day(1);

    return 0;
}
