//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified
    
    // import constants
    #include "encrypter.h"

    .section .rodata
.Lmsg: .string "wrbuf: Write failed on output\n"
.Lmsg2: .string "wrbuf: Buffer size error\n"

    .text

    ///////////////////////////////////////////////////
    // int wrbuf(char *IOBUF, int cnt, FILE *FPOUT); //
    // return cnt on success EXIT_FAIL otherwise     //
    ///////////////////////////////////////////////////
    .type   wrbuf, %function
    .global wrbuf
    .equ    FP_OFF, 12

/* 
 * Preserved Register Use: 
 * r4 contains cnt
 */
wrbuf:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF

    cmp     r1, 0             // compare cnt with 0
    bgt     .Lsecondif
    ldr     r0, =.Lmsg2
    bl      errmsg
    mov     r0, EXIT_FAIL     // return EXIT_FAIL
    b       .Ldone

.Lsecondif:
    mov     r4, r1            // temporarily store cnt in r4
    mov     r3, r2            // move FPOUT into r3
    mov     r2, r1            // move cnt into r2
    mov     r1, 1
    bl      fwrite

    cmp     r0, r4            // compare fwrite return to cnt
    beq     .Lcnt             
    ldr     r0, =.Lmsg        // load in msg2 into r0
    bl      errmsg
    mov     r0, EXIT_FAIL     // return EXIT_FAIL 
    b       .Ldone

.Lcnt:
    mov    r0, r4             // move cnt back into r0 to return

.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size   wrbuf, (. - wrbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
