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
 * function free_tickets()

 * Operation: tears down the database freeing all allocated memory. 
 *            Goes down each hash chain and free all the vehicles and 
 *            the tickets for each vehicle
 * params:
 *  hashtab   pointer to hashtable (pointer to an array of pointers)
 *  tabsz     number of elements in the hash table
 */
void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{
    unsigned long cnt = 0; /* number of tickets freed */

    /*
     * Loop through each item in hashtable
     * and free each linked list
     */
    for(uint32_t i = 0; i < tabsz; i++) {

        if(*(hashtab + i) != NULL) {
            struct vehicle *ptr = *(hashtab + i); /* pointer walker */

            /*
             * While ptr is not null,
             * free memory for current vehicle
             */
            while(ptr != NULL) {
                struct vehicle *next = ptr->next; /* next vehicle */

                free(ptr->plate);
                free(ptr->state);
            
                struct ticket *ticketHead = ptr->head; /* ticket walker */
                
                /*
                 * While ticket is not null,
                 * free memory for current ticket
                 */
                while(ticketHead != NULL) {
                    struct ticket *nextTicket = ticketHead->next; 
                    free(ticketHead);
                    ticketHead = nextTicket;
                    cnt++;
                }

                free(ptr);
                ptr = next;
            }
        }

    }
    free(hashtab);

    /*
     * Print out total number of tickets freed
     */
    printf("Total tickets freed: %lu\n", cnt);
}
#endif
