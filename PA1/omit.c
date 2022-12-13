/*
 * Programming assignment 1 
 *
 * remove c comments from a source file and write to stdout
 * comments are replaced with a single space
 * handle single and double quoted sequences
 * comments and quoted sequences can span multiple lines
 *
 * error handling:
 *     prints starting line for unterminated comments or quote sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h"


/*
 * function main(void)
 *
 * Operation: reads in characters
 * from a file and replaces comments
 * with a space. Outputs the replaced
 * contents to console. 
 *
 * params: none
 * returns: EXIT_SUCCESS if no unterminated
 * character or string literals and comments.
 * Otherwise returns EXIT_FAILURE.
 */
int 
main(void)
{
    int c; // store character read from input

    int lineNumber = 1; // the current line number on in file
    int startLine = 1; // start line of string, character, or comment

    enum typestate state = START; // store current state

    while((c = getchar()) != EOF) {
        switch(state) {
        case START:
            state = STARTst(c);
            startLine = lineNumber;
            break;
        case CHARLITERAL:
            state = CHARLITERALst(c);
            break;
        case STRINGLITERAL:
            state = STRINGLITERALst(c);
            break;
        case CHARBACKSLASH:
            state = CHARBACKSLASHst(c);
            break;
        case STRINGBACKSLASH:
            state = STRINGBACKSLASHst(c);
            break;
        case FORWARDSLASH:
            state = FORWARDSLASHst(c);
            startLine = lineNumber;
            break;
        case TWOFORWARDSLASH:
            state = TWOFORWARDSLASHst(c);
            break;
        case FORWARDSLASHASTERIK:
            state = FORWARDSLASHASTERIKst(c);
            break;
        case FORWARDSLASHTWOASTERIK:
            state = FORWARDSLASHTWOASTERIKst(c);
            break;
        default:
            fprintf(stderr, "Program error, DFA state not handled\n");
            return EXIT_FAILURE;
            break;
        }

        if(c == '\n') { 
            lineNumber++;
        }
    }

    /*
     * Print out a forward slash 
     * or else it will be left out
     */
    if(state == FORWARDSLASH) { 
        putchar('/');
    }

    /*
     * Handle unterminated character literal
     */
    if(state == CHARLITERAL || state == CHARBACKSLASH) {
        fprintf(stderr, "Error: line %d: unterminated quote(\')\n", startLine);
        return EXIT_FAILURE;
    }

    /*
     * Handle unterminated string literal
     */
    if(state == STRINGLITERAL || state == STRINGBACKSLASH) { 
        fprintf(stderr, "Error: line %d: unterminated quote(\")\n", startLine);
        return EXIT_FAILURE;
    }

    /*
     * Handle unterminated C++ comment
     */
    if(state == FORWARDSLASHASTERIK || state == FORWARDSLASHTWOASTERIK) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", startLine);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
