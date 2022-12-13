#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//uncomment the next line when you want to use your routine
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * function vehicle_lookup()
 * 
 * Operation: looks for vehicle in the database
 *            that matches both plate and state string
 *            arguments. 
 * params:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  argv    command line arguments
 *
 * returns:  pointer to vehicle if found NULL otherwise
 */

struct vehicle *
vehicle_lookup(struct vehicle **hashtab, uint32_t tabsz, char *plate,
               char *state, char **argv)
{
    uint32_t hashIdx = hash(plate, argv) % tabsz; /* index into hash table */

    struct vehicle *head = hashtab[hashIdx]; /* head of linked list */

    /*
     * Traverse linked list and find vehicle that
     * matches plate and state
     */
    while(head != NULL) {
        /*
         * Car found so return
         * pointer
         */
        if(strcmp(head->plate, plate) == 0 && 
           strcmp(head->state, state) == 0) {
            return head;
        }
        head = head->next;
    }

    /*
     * Vehicle not found
     */
    return NULL;
}
#endif
