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
 * function del_ticket()
 * 
 * Operation: remove ticket from the database by 
 *            finding the vehicle and the ticket with 
 *            the summons passed in as argument. If
 *            the vehicle ends up having 0 tickets
 *            remove the vehicle too.
 *
 * Params:
 *  hashtab   pointer to hashtable (pointer to an array of pointers)
 *  tabsz     number of elements in the hash table
 *  fineTab   table mapping fine code to description and fine   
 *  plate     plate id string to be found
 *  state     state id string to be found
 *  summ      summon id string to be found
 *  argv      command line arguments
 *
 * returns: 0 if no errors or -1 if not found or error
 */

int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
    uint32_t hashIdx = hash(plate, argv) % tabsz; /* index into hashtable */

    struct vehicle *vehicleHead = hashtab[hashIdx]; /* vehicle list head */
    struct vehicle *prevVehicle = vehicleHead; /* previous vehicle */

    /*
     * Try to find vehicle 
     * by traversing linked list
     */
    while(vehicleHead != NULL) {
        if(strcmp(vehicleHead->plate, plate) == 0 && 
           strcmp(vehicleHead->state, state) == 0) 
            break;
        
        prevVehicle = vehicleHead;
        vehicleHead = vehicleHead->next;
    }

    /**
     * Vehicle not found,
     * return -1
     */
    if(vehicleHead == NULL)
        return -1;

    unsigned long long convertSummons; /* summons id */

    if((strtosumid(summ, &convertSummons, argv)) != 0)
        return -1;

    struct ticket *ticketHead = vehicleHead->head; /* ticket list head */
    struct ticket *prev = ticketHead; /* previous ticket */

    /*
     * Try to find ticket 
     * by traversing linked list
     */
    while(ticketHead != NULL) {
        if(ticketHead->summons == convertSummons) 
            break;
        
        prev = ticketHead;
        ticketHead = ticketHead->next;
    }

    struct ticket *ticketToFree = ticketHead; /* store possible ticket */

    /**
     * If ticketToFree is NULL,
     * return -1. Otherwise, if it is
     * head of vehicle ticket list,
     * update vehicle head to 
     * ticketToFree's next. Otherwise,
     * update prev ticket's next to 
     * ticketToFree's next.
     */
    if(ticketToFree == NULL) 
        return -1;
    else if(ticketToFree == vehicleHead->head) 
        vehicleHead->head = ticketToFree->next;
    else 
        prev->next = ticketToFree->next;

    vehicleHead->cnt_ticket -= 1;
    vehicleHead->tot_fine -= fineTab[ticketToFree->code].fine;

    free(ticketToFree);
    ticketToFree = NULL;
    
    /*
     * Free vehicle if no
     * more tickets left on it.
     */
    if(vehicleHead->cnt_ticket == 0) {
        struct vehicle *vehicleToFree = vehicleHead; /* vehicle to free */

        if(vehicleHead == hashtab[hashIdx]) 
            hashtab[hashIdx] = vehicleHead->next;
        else 
            prevVehicle->next = vehicleHead->next;

        free(vehicleToFree->plate);
        free(vehicleToFree->state);
            
        struct ticket *ticketHead = vehicleToFree->head; /* ticket walker */

        /*
         * Free any tickets
         * that may exist 
         */
        while(ticketHead != NULL) {
            struct ticket *nextTicket = ticketHead->next;
            free(ticketHead);
            ticketHead = nextTicket;
        }

        free(vehicleToFree);
    }

    return 0;
}
#endif
