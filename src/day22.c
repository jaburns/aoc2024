#include "main.h"

internal u64 day22_step(u64 num) {
    num ^= num << 6;
    num &= 0xFFFFFF;
    num ^= num >> 5;
    num &= 0xFFFFFF;
    num ^= num << 11;
    num &= 0xFFFFFF;
    return num;
}

structdef(Day22Buyer) {
    i8 prices[2001];
};
DefArrayTypes(Day22Buyer);

internal DayResult day22(Arena* arena, Str input) {
    Vec_Day22Buyer buyers = VecAlloc(Day22Buyer, arena, 2048);
    u64            part1  = 0;
    u64            part2  = 0;

    foreach (StrSplitIter, it, '\n', input) {
        u64         num   = str_parse_u64(it.item, 10);
        Day22Buyer* buyer = VecPush(buyers);
        buyer->prices[0]  = num;
        for (u32 i = 1; i <= 2000; ++i) {
            num              = day22_step(num);
            buyer->prices[i] = num % 10;
        }
        part1 += num;
    }

    i8 kernel[4] = {-9, -9, -9, -9};
    for (;;) {
        u64 bananas = 0;

        for (u32 i = 0; i < buyers.count; ++i) {
            for (u32 j = 4; j <= 2000; ++j) {
                i8 a0 = buyers.items[i].prices[j - 4];
                i8 a1 = buyers.items[i].prices[j - 3];
                i8 a2 = buyers.items[i].prices[j - 2];
                i8 a3 = buyers.items[i].prices[j - 1];
                i8 a4 = buyers.items[i].prices[j];

                i8 d0 = a1 - a0;
                i8 d1 = a2 - a1;
                i8 d2 = a3 - a2;
                i8 d3 = a4 - a3;

                if (d0 == kernel[0] && d1 == kernel[1] && d2 == kernel[2] && d3 == kernel[3]) {
                    bananas += a4;
                    break;
                }
            }
        }

        if (bananas > part2) {
            part2 = bananas;
        }

        if (++kernel[0] > 9) {
            kernel[0] = -9;
            if (++kernel[1] > 9) {
                kernel[1] = -9;
                printf("%i %i\n", kernel[2], kernel[3]);
                if (++kernel[2] > 9) {
                    kernel[2] = -9;
                    if (++kernel[3] > 9) {
                        goto done;
                    }
                }
            }
        }
    }
done: {}

    DayResult result       = {0};
    result.parts[0].as_i64 = part1;
    result.parts[1].as_i64 = part2;
    return result;
}