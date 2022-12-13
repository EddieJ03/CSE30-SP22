/* 
 * the next two lines are the header guards
 */
#ifndef _OMIT_H
#define _OMIT_H
/*
 * header file for omit
 * type definition (enum) for DFA states
 * function prototypes
 */
enum typestate {START, CHARLITERAL, STRINGLITERAL, CHARBACKSLASH, 
                STRINGBACKSLASH, FORWARDSLASH, TWOFORWARDSLASH, 
                FORWARDSLASHASTERIK, FORWARDSLASHTWOASTERIK};

/* Prototypes Here: */
enum typestate STARTst(int);
enum typestate CHARLITERALst(int);
enum typestate STRINGLITERALst(int);
enum typestate CHARBACKSLASHst(int);
enum typestate STRINGBACKSLASHst(int);
enum typestate FORWARDSLASHst(int);
enum typestate TWOFORWARDSLASHst(int);
enum typestate FORWARDSLASHASTERIKst(int);
enum typestate FORWARDSLASHTWOASTERIKst(int);

#endif
