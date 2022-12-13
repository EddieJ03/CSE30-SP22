#include <stdio.h>
#include <stdlib.h>
#include "cw.h"

char buffer[BUFSZ];
int  cntchar;   // count of chars IN words,not \n, \t or ' '
int  cntword;   // number of words
int  cntline;   // number of \n

int setup(int, char **);
int rd(void);
void result(void); 

/*
 * usage: ccw [file]
 *
 * function main()
 *
 * Operation: read in line of characters
 *            using rd() method. Then walk
 *            the buffer and update cntword,
 *            cntline, and cntchar appropriately.
 *            Print out the resulting counts.
 *
 * params: argc and argv. argc is the number of 
 *         arguments and argv is an array of the 
 *         command line arguments.
 *
 * returns: EXIT_SUCCESS if no issue
 *          otherwise EXIT_FAILURE
 */
int
main(int argc, char **argv)
{
    if (setup(argc, argv) != 0)
        return EXIT_FAILURE;
    
    /*
     * in a loop until end of file
     * call rd() to get a buffer of chars 
     * rd() returns the number of valid chars in the buffer
     * <= 0 signals end of file
     * the buffer contains an array of chars, not a string
     */
    int len = 0; /* length of buffer */
    int wordLen = 0; /* running length of possible word */

    while ((len = rd()) > 0) {
        char *end = buffer + len; /* end pointer */

        /*
        * Walk through buffer. If see a space, tab, or newline
        * check if wordLen is greater than 0. If it is, we saw
        * a word so update cntword and rest wordLen to 0. If
        * we saw a newline update cntline. Otherwise increment
        * both wordLen and cntchar.
        */
        for (char *ptr = buffer; ptr < end; ptr++) {
            if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                if (wordLen > 0) {
                    cntword++;
                    wordLen = 0;
                }

                if (*ptr == '\n') {
                    cntline++;
                }
            } else {
                wordLen++;
                cntchar++;
            }
        }
    }

    /*
    * Possibly saw another
    * word that was reached
    * after finishing loop
    */
    if (wordLen > 0) {
        cntword++;
        wordLen = 0;
    }

    /*
     * result() prints the summary of the values in the global
     * variables above
     * Until you complete result.S you can uncomment #define _PA4DBG_
     * to test your code make sure #define _PA4DBG_ is commented out
     * when you submit your code.
     */
//#define _PA4DBG_
#ifdef _PA4DBG_
     printf("lines:%8.8d words:%8.8d Chars:%8.8d\n", cntline,cntword,cntchar);
#else
    result();
#endif
    return EXIT_SUCCESS;
}
