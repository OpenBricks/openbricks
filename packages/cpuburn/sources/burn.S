        .arch   armv7-a
        .fpu    neon

        .text

        .global main
        .type   main, STT_FUNC
        .func   main
main:
        bic             sp,  sp,  #15
1:
        vld1.8          {q8}, [sp,:128]
        smuad           r6,  r4,  r5
        veor            q9,  q0,  q1
        add             lr,  r4,  r5
        ldr             ip,  [sp]
        vext.8          q10, q3,  q4,  #1
        eor             r5,  r4,  r5
        vadd.i32        q11, q0,  q1
        smusd           r6,  r4,  r5
        ldr             r2,  [sp]
        vext.8          q12, q1,  q2,  #3
        ssub8           lr,  r4,  r5
        vmul.u16        q13, q0,  q4
        b               1b
.endfunc
