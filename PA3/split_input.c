#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "misc.h"
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * split_input
 * 
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * returns:  0 if all the columns were found,  -1 otherwise
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
    char* ptr = buf; // pointer to walk buffer

    int col_counter; // keep track of field count

    /*
     * Continue loop until cnt is reached or ptr points
     * to a null character.
     */
    for (col_counter = 0; *ptr != '\0' && col_counter < cnt; col_counter++) {
        *(table + col_counter) = ptr; // store pointer in table
    
        /*
         * If first character is a quote,
         * process as quoted field. Otherwise,
         * process as
         */
        if (*ptr == '\"') {
            int seen_end = 0; // flag if final quote seen

            /*
             * Increment ptr one to skip current quote. 
             * Until ptr points to a null character, 
             * process the current character and then 
             * increment ptr.
             */
            for (ptr = ptr + 1; *ptr != '\0'; ptr++) {
                /**
                 * If current character is quote, 
                 * check next character. Otherwise,
                 * if seen final quote, check if
                 * field is done.
                */
                if (*ptr == '\"') { 
                    /*
                     * Check next character. If next character is
                     * the delimeter, new line, or null 
                     * character we are at a final quote.
                     * If it a quote, increment ptr. Otherwise
                     * return -1.
                     */
                    if (*(ptr + 1) == delim || *(ptr + 1) == '\n' || 
                        *(ptr + 1) == '\0') { 
                        seen_end = 1;
                    } else if (*(ptr + 1) == '\"') { 
                        ptr++;
                    } else {
                        dropmsg("Quoted field not terminated properly", 
                                lineno, argv);
                        return -1;
                    }
                } else if (seen_end == 1) { 
                    /*
                     * End of current field so 
                     * terminate with null character
                     */
                    if ((*ptr == delim) || (*ptr == '\n')) { 
                        *ptr++ = '\0';
                        break;
                    }
                }
            }
            
	    /*
             * Did not see ending quote
             */
            if (seen_end == 0) { 
                dropmsg("Quoted field missing final quote", 
                        lineno, argv);
                return -1;
            }
        } else {
            /*
             * Continue loop until ptr points
             * to a null character.
             */
            for (; *ptr != '\0'; ptr++) {
                /*
                 * If see a quote return -1
                 */
                if (*ptr == '\"') { 
                    dropmsg("A \" is not allowed inside unquoted field", 
                            lineno, argv);
                    return -1;
                }
                
                /*
                 * End of current field so 
                 * terminate with null character
                 */
                if ((*ptr == delim) || (*ptr == '\n')) { 
                    *ptr++ = '\0';
                    break;
                }
            }
        }
    }

    /*
     * Check if exact number of columns,
     * too many columns, or too few
     * columns. 
     */
    if (*ptr == '\0' && col_counter == cnt) { 
        return 0;
    } else if (col_counter < cnt) { 
        dropmsg("too few columns", lineno, argv);
        return -1;
    } else { 
        dropmsg("too many columns", lineno, argv);
        return -1;
    }
}
#endif
