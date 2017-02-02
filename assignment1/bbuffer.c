/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bbuffer.h"

/*
 * Declarations for bounded-buffer shared variables -- plus concurrency-control
 * variables -- must START here.
 */


void initialize_bounded_buffer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */

}


void add_to_buffer(int value) {
    /* TO DO */
}


int remove_from_buffer() {
    /* TO DO */

    return -1;
}
