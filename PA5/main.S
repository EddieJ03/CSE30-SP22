//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // function import
    .extern encrypt
    .extern decrypt
    .extern setup
    .extern rdbuf
    .extern cleanup
    .extern wrbuf

    // global constants
    #include "encrypter.h"

    .text

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // encrypter [-d | -e] -b bookfile encryption_file  //
    //////////////////////////////////////////////////////

    .global main
    .type   main, %function
    .equ    BUFSZ,      1024         // buffer size
    .equ    FP_OFF,     28           // fp offset in main stack frame
    .equ    FPBOOK,     FP_OFF+4     // FILE * to book file
    .equ    FPIN,       FPBOOK+4     // FILE * to input file
    .equ    FPOUT,      FPIN+4       // FILE * to output file
    .equ    MODE,       FPOUT+4      // decrypt or encrypt mode
    .equ    IOBUF,      MODE+BUFSZ   // buffer for input file
    .equ    BOOKBUF,    IOBUF+BUFSZ  // buffer for book file
    .equ    PAD,        BOOKBUF+0    // Stack frame PAD 
    .equ    OARG6,      PAD+4        // output arg6
    .equ    OARG5,      OARG6+4      // output arg5 must be at bottom
    .equ    FRAMESZ,    OARG5-FP_OFF // total space for frame
 
/*  
 * Preserved Register Use:             
 * r4 pointer to input/output buffer  
 * r5 pointer to book buffer          
 * r6 contains mode flag                       
 * r7 contains input FILE *                    
 * r8 contains book FILE *                     
 * r9 contains output FILE *                  
 */
main:
    push    {r4-r9, fp, lr}
    add     fp, sp, FP_OFF          // set frame pointer to frame base
    ldr     r3, =FRAMESZ            // frame size is big, use pseudo ldr
    sub     sp, sp, r3              // allocate space for locals and passed args 
     
    // call int setup(argc, argv, *mode, *FPBOOK, *FPIN, *FPOUT)
    // set up registers for function call to setup
    sub     r9, fp, FPOUT
    str     r9, [fp, -OARG6]         // store FPOUT as 6th argument
     
    sub     r7, fp, FPIN
    str     r7, [fp, -OARG5]         // store FPIN as 5th argument

    sub     r2, fp, MODE             // put address of MODE in r2
    sub     r3, fp, FPBOOK           // put address of FPBOOK in r3

    bl      setup
    cmp     r0, EXIT_FAIL
    bne     .Lbefore
    mov     r0, EXIT_FAILURE         // return EXIT_FAILURE
    b       .Lexit

.Lbefore:
    // set up for main loop
    ldr     r8, =IOBUF
    sub     r4, fp, r8               // r4 contains I/O buffer starting address
    ldr     r8, =BOOKBUF
    sub     r5, fp, r8               // r5 contains BOOKBUF starting address
    ldr     r6, [fp, -MODE]          // r6 contains MODE
    ldr     r9, [r9]                 // r9 contains FPOUT
    ldr     r8, [fp, -FPBOOK]        // r8 contains FPBOOK
    ldr     r7, [r7]                 // r7 contains FPIN

/*
 * Repeatedly call rdbuf until it returns 
 * value less than or equal to 0
 */
.Lloop:
    ldr     r0, =BUFSZ
    str     r0, [fp, -OARG5]
    mov     r0, r4                   // put IOBUF address in r0
    mov     r1, r5                   // put BOOKBUF address in r1  
    mov     r2, r7                   // put FPIN in r2
    mov     r3, r8                   // put FPBOOK in r3

    bl      rdbuf
    cmp     r0, 0
    ble     .Ldone

.Lprocinput:
    // Setup for call to encrypt/decrypt
    mov     r2, r0                   // put cnt in r2
    mov     r0, r4                   // put IOBUF in r0
    mov     r1, r5                   // put BOOKBUF in r1
    cmp     r6, ENCRYPT_MODE
    bne     .Ldecrypt
    bl      encrypt
    b       .Lwriteout

.Ldecrypt: 
    bl      decrypt

.Lwriteout:
    // XXcrypted the buffer, write it out
    cmp     r0, EXIT_FAIL
    beq     .Ldone
    mov     r1, r0                   // put cnt in r1
    mov     r0, r4                   // put IOBUF in r0
    mov     r2, r9                   // put FPOUT in r2
    bl      wrbuf
    cmp     r0, EXIT_FAIL
    beq     .Ldone
    b       .Lloop

.Ldone:
    // int cleanup(int status)
    // r0 should have exit status in it
    bl      cleanup

.Lexit:
    sub     sp, fp, FP_OFF
    pop     {r4-r9, fp, lr}
    bx      lr

    .size   main, (. - main)
    .section .note.GNU-stack,"",%progbits
.end
#endif
