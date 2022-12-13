//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/22/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // global constants
    #include "encrypter.h"

    .section .rodata
.Lmsg:  .string "rdbuf: Bookfile is too short for message\n"
.Lmsg2: .string "rdbuf: Buffer size error\n"
    .text

    //////////////////////////////////////////////////////
    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN
    //              FILE *FPBOOK, int BUFSZ);
    // returns number of bytes read EXIT_FAIL otherwise
    //////////////////////////////////////////////////////

    .type   rdbuf, %function
    .global rdbuf
    .equ    IARG5,  4 
    .equ    FP_OFF, 12

/*
 * Preserved Register Use: 
 * r4 contains BOOKBUF
 * r5 contains FPBOOK and saved cnt
 */
rdbuf:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF

    ldr     r4, [fp, IARG5]   // load in BUFSZ into r4
    cmp     r4, 0             // ompare BUFSZ with 0
    bgt     .Lsecondif        

    ldr     r0, =.Lmsg2       // put .Lmsg2 into r0
    bl      errmsg
    mov     r0, EXIT_FAIL     // return EXIT_FAIL 
    b       .Lend

.Lsecondif:
    mov     r4, r1            // temporarily store BOOKBUF in r4
    mov     r5, r3            // temporarily store FPBOOK in r5

    mov     r1, 1             
    mov     r3, r2            // move FPIN to r3
    ldr     r2, [fp, IARG5]   // load in BUFSZ into r2

    bl      fread
    cmp     r0, 0             // compare return value of fread with 0
    bne     .Lthirdif         
    b       .Lend

.Lthirdif:
    mov     r3, r5            // store FPBOOK in r3
    mov     r2, r0            // move cnt into r2
    mov     r1, 1
    mov     r5, r0            // move cnt into r5
    mov     r0, r4            // move BOOKBUF into r0
    
    bl      fread
    cmp     r0, r5            // cmp fread return value with cnt in r5
    beq     .Lcnt
    ldr     r0, =.Lmsg
    bl      errmsg
    mov     r0, EXIT_FAIL     // return EXIT_FAIL
    b       .Lend

.Lcnt:
    mov     r0, r5            // move cnt in r5 back into r0 to return

.Lend:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size   rdbuf, (. - rdbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
