/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h"

/*
 * function STARTst(int c)
 *
 * Operation: Gives the next state
 * if currently in the START
 * state.
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate STARTst(int c) 
{
    if(c == '/') 
        return FORWARDSLASH;

    putchar(c);

    if(c == '\'') 
        return CHARLITERAL;
    else if(c == '\"') 
        return STRINGLITERAL;

    return START;
}

/*
 * function CHARLITERALst(int c)
 *
 * Operation: Gives the next state
 * if currently in the CHARLITERAL
 * state (we have seen a character
 * literal).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate CHARLITERALst(int c) 
{
    putchar(c);

    if(c == '\\') 
        return CHARBACKSLASH;
    else if(c == '\'') 
        return START;
    
    return CHARLITERAL;
}

/*
 * function STRINGLITERALst(int c)
 * 
 * Operation: Gives the next state
 * if currently in the STRINGLITERAL
 * state (we have seen a string
 * literal).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate STRINGLITERALst(int c) 
{
    putchar(c);

    if(c == '\\') 
        return STRINGBACKSLASH;
    else if(c == '\"') 
        return START;

    return STRINGLITERAL;
}

/*
 * function CHARBACKSLASHst(int c)
 *
 * Operation: Gives the next state
 * if currently in the CHARBACKSLASH
 * state (we have seen a backslash).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate CHARBACKSLASHst(int c) 
{
    putchar(c);
    return CHARLITERAL;
}

/*
 * function STRINGBACKSLASHst(int c)
 *
 * Operation: Gives the next state
 * if currently in the STRINGBACKSLASH
 * state (we have seen a backslash).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate STRINGBACKSLASHst(int c) 
{
    putchar(c);
    return STRINGLITERAL;
}

/*
 * function FORWARDLSASHst(int c)
 *
 * Operation: Gives the next state
 * if currently in the FORWARDSLASH
 * state (we have seen a forward slash).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate FORWARDSLASHst(int c) 
{
    if(c == '/') {
        putchar(' ');
    return TWOFORWARDSLASH;
    } else if(c == '*') {
        putchar(' ');
        return FORWARDSLASHASTERIK;
    } else {
        putchar('/');
        putchar(c);
        return START;
    }
}

/*
 * function TWOFORWARDSLASHst(int c)
 *
 * Operation: Gives the next state
 * if currently in the TWOFORWARDSLASH
 * state (we have seen two forward slashes).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate TWOFORWARDSLASHst(int c) 
{
    if(c == '\n') {
        putchar('\n');
        return START;
    }
    return TWOFORWARDSLASH;
}

/*
 * function FORWARDSLASHASTERIKst(int c)
 *
 * Operation: Gives the next state
 * if currently in the FORWARDSLASHASTERIK
 * state (we have seen a forward slash and
 * an asterik).
 *
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate FORWARDSLASHASTERIKst(int c) 
{
    if(c == '*') 
        return FORWARDSLASHTWOASTERIK;
    else if(c == '\n') 
        putchar('\n');
     
    return FORWARDSLASHASTERIK;
}

/*
 * function FORWARDSLASHTWOASTERIKst(int c)
 *
 * Operation: Gives the next state
 * if currently in the FORWARDSLASHTWOASTERIK
 * state (we have seen a forward slash and
 * two asteriks).
 *
 * param: an integer c
 * return: the next typestate
 */
enum typestate FORWARDSLASHTWOASTERIKst(int c) 
{
    if(c == '/') 
        return START;
    else if(c == '*') 
        return FORWARDSLASHTWOASTERIK;
    else if(c == '\n') 
        putchar('\n');
     
    return FORWARDSLASHASTERIK;
}
