//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/22/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "encrypter.h"

    .text     // start of text segment

    //////////////////////////////////////////////////
    // int encrypt(char *IOBUF, char *BOOKBUF, int cnt)
    // encrypts  IOBUF with BOOKBUF; updating IOBUF
    //////////////////////////////////////////////////
    .global encrypt
    .type   encrypt, %function
    .equ    FP_OFF, 12

/* Register Use:
 * r3 stores loop counter
 * r4 stores byte from IOBUF
 * r5 stores byte from BOOKBUF 
 */
encrypt:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF

    mov      r3, 0           // loop counter
    cmp      r3, r2          // pre-loop guard
    bge      .Lend

.Lfor:   
    ldrb     r4, [r0, r3]    // load in byte from IOBUF
    mov      r5, r4          // move copy of byte from IOBUF into r4
    lsl      r5, r5, 28      
    lsr      r5, r5, 24      // last 4 bits 
    lsr      r4, r4, 4       // first 4 bits
    orr      r4, r4, r5      // merge bits
    ldrb     r5, [r1, r3]    // load in byte from BOOKBUF
    eor      r4, r4, r5
    strb     r4, [r0, r3]    // store XORed byte back into IOBUF
    add      r3, r3, 1       // increment loop counter
    cmp      r3, r2          
    blt      .Lfor

.Lend:
    mov     r0, r2 

.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr
    .size encrypt, (. - encrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
