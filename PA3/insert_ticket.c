#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"
#include "readsubs.h"

//uncomment the next line when you want to use your routine
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * function insert_ticket()
 *  
 * Operation: Add a ticket (summons) to the vehicle
 *            with the corresponding plate and state 
 *            at the end of its ticket list. If the vehicle 
 *            is not in the database then a new struct vehicle 
 *            entry must be made and added to front of
 *            of corresponding linked list in hash table.
 * 
 * params:
 *  hashtab   pointer to hashtable (pointer to an array of pointers)
 *  tabsz     number of elements in the hash table
 *  fineTab   pointer fine table. maps code number to text description 
 *            & fine cost you use this table to get the amount of the 
 *            fine when calculating the tot_fine field in struct vehicle.
 * 
 *  summ      summons id string to be added        
 *  plate     plate id string to be added
 *  state     state id string to be added
 *  date      date of summons string
 *  code      summons code integer value used an an index into the fines table
 *  argv      command line arguments
 *
 * returns: 0 if ok -1 for all errors
 */

int
insert_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab, 
    char *summ, char *plate, char *state, char *date, int code, char **argv)
{
    uint32_t hashedIdx = hash(plate, argv) % tabsz; /* index into hashtable */
    
    unsigned long long summid; /* summons id */
    
    if(strtosumid(summ, &summid, argv) != 0) 
        return -1;

    time_t dateval; /* converted date */
     
    if(strtoDate(date, &dateval, argv) != 0) 
        return -1;
    
    struct vehicle *walker = hashtab[hashedIdx]; /* walk vehicle list */
    struct vehicle *foundCar = NULL; /* vehicle to insert on */

    /*
     * Try to find vehicle 
     * by traversing linked list
     */
    while(walker != NULL) {
        if(strcmp(walker->state, state) == 0 && 
           strcmp(walker->plate, plate) == 0) {
            foundCar = walker;
            break;
        }
        walker = walker->next;
    }

    struct ticket *newTicket = malloc(sizeof(*newTicket)); /* new ticket */

    if(newTicket == NULL) 
        return -1;

    /*
     * Assign fields of
     * ticket
     */
    newTicket->summons = summid;
    newTicket->date = dateval;
    newTicket->code = code;
    newTicket->next = NULL;

    if(foundCar == NULL) {
        foundCar = malloc(sizeof(struct vehicle));

        /*
         * Unable to allocate memory
         * for vehicle
         */
        if(foundCar == NULL) {
            free(newTicket);
            newTicket = NULL;
            return -1;
        }

        /*
         * Have plate field point to
         * plate duplicate. If unable to
         * free up allocated memory
         * and return -1.
         */
        if((foundCar->plate = strdup(plate)) == NULL) {
            free(foundCar);
            free(newTicket);
            return -1;
        }

        /*
         * Have state field point to
         * state duplicate. If unable to
         * free up allocated memory
         * and return -1.
         */
        if((foundCar->state = strdup(state)) == NULL) {
            free(foundCar->plate);
            free(newTicket);
            free(foundCar);
            return -1;
        }
        
        foundCar->cnt_ticket = 0;
        foundCar->tot_fine = 0;
        foundCar->head = NULL;

        /*
         * Insert vehicle at front
         * of its list and update
         * hashtable pointer
         */
        foundCar->next = hashtab[hashedIdx];
        hashtab[hashedIdx] = foundCar;
    } 
    
    foundCar->cnt_ticket += 1;
    foundCar->tot_fine += fineTab[code].fine;

    struct ticket *ptr = foundCar->head; /* ticket list head */
    struct ticket *prev = ptr; /* previous ticket */

    /*
     * Walk through ticket list
     * until end
     */
    while(ptr != NULL) {
        prev = ptr;
        ptr = ptr->next;
    }

    /*
     * Add ticket to end
     * of car ticket list
     */
    if(prev == NULL) 
        foundCar->head = newTicket;
    else 
        prev->next = newTicket;

    return 0;
}
#endif
