#ifndef _BBUFFER_H_
#define _BBUFFER_H_

#define BOUNDED_BUFFER_SIZE 4

void initialize_bounded_buffer();
void add_to_buffer(int);
int  remove_from_buffer();

#endif
