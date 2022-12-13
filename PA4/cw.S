    .arch armv6             // armv6 architecture
    .arm                    // arm 32-bit instruction set
    .fpu vfp                // floating point co-processor
    .syntax unified         // modern sytax

    // include files
    #include    "cw.h"

    // name imports
    .extern     setup
    .extern     rd
    .extern     result
    
    // name exports
    .global     buffer
    .global     cntchar
    .global     cntword
    .global     cntline

    .bss       // segment
    // allocate space for (1) buffer, (2) cntchar (3) cntword
    // (4) cntline
    cntline:    .word 0
    cntchar:    .word 0 
    cntword:    .word 0
    buffer:     .space BUFSZ

    .text       // segment

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // program usage: cw [file]                         //
    //////////////////////////////////////////////////////

    .global main                // global for linking to
    .type   main, %function     // define as a function
    .equ    EXIT_SUCCESS, 0
    .equ    EXIT_FAILURE, 1
    .equ    FP_OFF, 20          // fp offset in stack frame

/* r0 is the length of the buffer
 * r1 tracks the length of a seen word
 * r2 contains current character to process
 * r3 contains the end pointer
 * r4 store cntline
 * r5 store cntword
 * r6 store cntchar
 * r7 contains buffer walker
 */
main:
    // function prologue
    push    {r4-r7,fp, lr}      // save fp and lr on stack
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    bl      setup               // call setup(argc, argv)
    cmp     r0, 0               // returns 0 if ok, -1 otherwise
    beq     .Lok
    mov     r0, EXIT_FAILURE
    b       .Lexit

.Lok:
    mov     r1, 0               // store word length 
    mov     r4, 0               // store cntline
    mov     r5, 0               // store cntword
    mov     r6, 0               // store cntchar

/*
 * In a loop call rd() until end of file
 */

.Lwhile:
    mov     r7, r1              // temporarilty store word length in r7
    bl      rd                  // read another buffer
    mov     r1, r7              // move word length back to r1
    cmp     r0, 0               // if r0 <= 0 then EOF
    ble     .Lout               // get out of outer loop that reads in buffer

    ldr     r7, =buffer         // store buffer walker
    add     r3, r7, r0          // set end pointer
    
    
.Linner:                        // loop to walk buffer
    cmp    r7, r3               
    bge    .Lwhile              // break out of inner loop
    
    /*
     * Load character in r2 
     * and check if tab, newline
     * or space.
     */
    ldrb   r2, [r7]  
    cmp    r2, '\t'  
    beq    .Ltrue    
    cmp    r2, ' '
    beq    .Ltrue
    cmp    r2, '\n'
    bne    .Lfalse

    /*
     * Character is a tab,
     * space, or newline.
     */
.Ltrue:                        
    cmp     r1, 0               // check if saw a word
    beq     .Lsecond_if
    mov     r1, 0               // reset length of word seen
    add     r5, r5, 1           // increment cntword

    /*
     * If newline character
     * increment cntline 
     */
.Lsecond_if:
    cmp     r2, '\n'
    bne     .Lendif
    add     r4, r4, 1  
    b       .Lendif  

    /*
     * Character is not a tab,
     * space, or newline.
     */
.Lfalse:
    add    r6, r6, 1            // increment cntchar
    add    r1, r1, 1            // increment length of word seen
    
.Lendif:
    add    r7, r7, 1            // increment buffer walker
    b      .Linner              // go back to start of inner loop

.Lout:
    cmp    r1, 0                // Possibly another word 
    beq    .Lstore              
    add    r5, r5, 1            // increment cntword       
    
    /*
     * Write cntline
     * cntword, and cntchar
     * values back out to
     * memory. r7 used to 
     * store address
     */
.Lstore:
    ldr     r7, =cntline
    str     r4, [r7]
    ldr     r7, =cntword
    str     r5, [r7]
    ldr     r7, =cntchar
    str     r6, [r7]

    bl      result              // print the results
    mov     r0, EXIT_SUCCESS    // fall through to .Lexit

.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4-r7,fp,lr}       // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size   main, (. - main)    // set size for function
.end
