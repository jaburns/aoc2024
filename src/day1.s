.section __TEXT,__text

; ======================================================================================================================
; DayResult day1(Arena* arena, Str input);
; x0 -> Arena*      arena
; x1 -> char*       input.items
; x2 -> size_t      input.count
; x8 -> DayResult*  result
.global _day1
_day1:
        mov x0, #12345
    ; write result into the returned struct and return
        str xzr, [x8       ]  ; part[0].is_str
        str x0,  [x8, #0x08]  ; part[0].as_u64
        str xzr, [x8, #0x18]  ; part[1].is_str
        str x0,  [x8, #0x20]  ; part[1].as_u64
        ret