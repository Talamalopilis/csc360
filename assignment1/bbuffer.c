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
int buffer[BOUNDED_BUFFER_SIZE];
int reader = 0;
int writer = 0;

int count = 0;
pthread_mutex_t buffer_mutex;
pthread_cond_t buffer_cond_removed;
pthread_cond_t buffer_cond_added;

void initialize_bounded_buffer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    int status;
    status = pthread_mutex_init(&buffer_mutex, NULL);
    if (status != 0) {
    	fprintf(stderr, "Error creating buffer_mutex\n");
    	exit(1);
    }

    status = pthread_cond_init(&buffer_cond_removed, NULL);
    if (status != 0) {
    	fprintf(stderr, "Error creating buffer_cond_removed\n");
    	exit(1);
    }

    status = pthread_cond_init(&buffer_cond_added, NULL);
    if (status != 0) {
    	fprintf(stderr, "Error creating buffer_cond_added\n");
    	exit(1);
    }
}


void add_to_buffer(int value) {
    pthread_mutex_lock(&buffer_mutex);
    while(count == BOUNDED_BUFFER_SIZE){
    	pthread_cond_wait(&buffer_cond_removed, &buffer_mutex);
    }
    buffer[writer] = value;
    writer = (writer+1) % BOUNDED_BUFFER_SIZE;
    count++;

    pthread_cond_broadcast(&buffer_cond_added);
    pthread_mutex_unlock(&buffer_mutex);
}


int remove_from_buffer() {
    pthread_mutex_lock(&buffer_mutex);
    while(count == 0){
    	pthread_cond_wait(&buffer_cond_added, &buffer_mutex);
    }
    int ret = buffer[reader];
    reader = (reader+1) % BOUNDED_BUFFER_SIZE;
    count--;

    pthread_cond_broadcast(&buffer_cond_removed);
    pthread_mutex_unlock(&buffer_mutex);

    return ret;
}
