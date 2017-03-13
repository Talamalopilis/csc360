/*
 * cpusched.c
 *
 * Skeleton code for solution to A#2, CSC 360, Spring 2017
 *
 * Prepared by: Michael Zastre (University of Victoria) 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_LINE_LENGTH 100

#define FCFS 0
#define PS   1
#define RR 2
#define STRIDE 3

#define PRIORITY_LEVELS 4


/*
 * Stores raw event data from the input,
 * and has spots for per-task statistics.
 * You may want to modify this if you wish
 * to store other per-task statistics in
 * the same spot.
 */

typedef struct Task_t {
    int   arrival_time;
    float length;
    int   priority;

    float finish_time;
    int   dispatches;
    float cpu_cycles;
} task_t; 


/*
 * Some function prototypes.
 */

void read_task_data(void);
void init_simulation_data(int);
void first_come_first_serve(void);
void stride_scheduling(int);
void priority_scheduling(void);
void rr_scheduling(int);
void run_simulation(int, int);
void compute_and_print_stats(void);

void simulate(int (*get_next_task)(int*, int, int), int quantum, int (*set_state)(int));
int get_min_priority_task(int*, int);
int get_next_rr_task(int*, int, int);
int set_state_rr(int);



/*
 * Some global vars.
 */
int     num_tasks = 0;
task_t *tasks = NULL;


void read_task_data()
{
    int max_tasks = 2;
    int  in_task_num, in_task_arrival, in_task_priority;
    float in_task_length;
    

    assert( tasks == NULL );

    tasks = (task_t *)malloc(sizeof(task_t) * max_tasks);
    if (tasks == NULL) {
        fprintf(stderr, "error: malloc failure in read_task_data()\n");
        exit(1);
    }
   
    num_tasks = 0;

    /* Given the format of the input is strictly formatted,
     * we can used fscanf .
     */
    while (!feof(stdin)) {
        fscanf(stdin, "%d %d %f %d\n", &in_task_num,
            &in_task_arrival, &in_task_length, &in_task_priority);
        assert(num_tasks == in_task_num);
        tasks[num_tasks].arrival_time = in_task_arrival;
        tasks[num_tasks].length       = in_task_length;
        tasks[num_tasks].priority     = in_task_priority;

        num_tasks++;
        if (num_tasks >= max_tasks) {
            max_tasks *= 2;
            tasks = (task_t *)realloc(tasks, sizeof(task_t) * max_tasks);
            if (tasks == NULL) {
                fprintf(stderr, "error: malloc failure in read_task_data()\n");
                exit(1);
            } 
        }
    }
}


void init_simulation_data(int algorithm)
{
    int i;

    for (i = 0; i < num_tasks; i++) {
        tasks[i].finish_time = 0.0;
        tasks[i].dispatches = 0;
        tasks[i].cpu_cycles = 0.0;
    }
}


void first_come_first_serve() 
{
    int current_task = 0;
    int current_tick = 0;

    for (;;) {
        current_tick++;

        if (current_task >= num_tasks) {
            break;
        }

        /*
         * Is there even a job here???
         */
        if (tasks[current_task].arrival_time > current_tick-1) {
            continue;
        }

        tasks[current_task].cpu_cycles += 1.0;
        
        if (tasks[current_task].cpu_cycles >= tasks[current_task].length) {
            float quantum_fragment = tasks[current_task].cpu_cycles -
                tasks[current_task].length;
            tasks[current_task].cpu_cycles = tasks[current_task].length;
            tasks[current_task].finish_time = current_tick - quantum_fragment;
            tasks[current_task].dispatches = 1;
            current_task++;
            if (current_task > num_tasks) {
                break;
            }
            tasks[current_task].cpu_cycles += quantum_fragment;
        }
    }
}


void stride_scheduling(int quantum)
{
    printf("STRIDE SCHEDULING appears here\n");
    exit(1);
}



void priority_scheduling()
{
    int parser = 0;
    int ready[num_tasks];
    int ready_size = 0;
    int current_tick = 0;
    int current_task = -1;

    while (1){
    	if (parser >= num_tasks && current_task == -1){
    		break;
    	}

    	while (tasks[parser].arrival_time <= current_tick && parser < num_tasks){
    		ready[parser] = tasks[parser].priority;
    		parser++;
    		ready_size++;
    	}

    	int new_task = get_min_priority_task(ready, ready_size);

    	current_tick++;
    	if (new_task == -1){
    		current_task = new_task;
    		continue;
    	}
    	if (new_task != current_task){
    		tasks[new_task].dispatches++;
    		current_task = new_task;
    	}

    	tasks[current_task].cpu_cycles += 1.0;
        
        if (tasks[current_task].cpu_cycles >= tasks[current_task].length) {
            float quantum_fragment = tasks[current_task].cpu_cycles -
                tasks[current_task].length;
            tasks[current_task].cpu_cycles = tasks[current_task].length;
            tasks[current_task].finish_time = current_tick - quantum_fragment;
            
            ready[current_task] = PRIORITY_LEVELS + 1; //completed
            int next_task = get_min_priority_task(ready, ready_size);
            if (next_task != -1){
            	tasks[next_task].cpu_cycles += quantum_fragment;
            }
        }
    }
}

int get_min_priority_task(int* ready, int ready_size){
	int min = PRIORITY_LEVELS + 1;
	int index = 0;
	int i;
	for (i = 0; i < ready_size; i++){
		if (ready[i] < min){
			index = i;
			min = ready[i];
		}
	}
	if (min == PRIORITY_LEVELS + 1){
		return -1;
	}
	return index;
}


void rr_scheduling(int quantum)
{
    simulate(get_next_rr_task, quantum, set_state_rr);
}


int get_next_rr_task(int* ready, int ready_size, int current_task){
	int any = 0;
	int index = 0;
	int i;
	for (i = 0; i < ready_size; i++){
		if (ready[(current_task + i + 1) % ready_size] == 1){
			index = (current_task + i + 1) % ready_size;
			any = 1;
			break;
		}
	}
	if (!any){
		return -1;
	}
	return index;
}

int set_state_rr(int task){
	if (task < 0){
		return 0;
	}
	else{
		return 1;
	}
}

void simulate(int (*get_next_task)(int*, int, int), int quantum, int (*set_state)(int)){
	int parser = 0;
    int ready[num_tasks];
    int ready_size = 0;
    int current_tick = 0;
    int current_task = -1;

    while (1){
    	if (parser >= num_tasks && current_task == -1){
    		break;
    	}

    	while (tasks[parser].arrival_time <= current_tick && parser < num_tasks){
    		ready[parser] = (*set_state)(parser);
    		parser++;
    		ready_size++;
    	}

    	int new_task = (*get_next_task)(ready, ready_size, current_task);

    	current_tick += quantum;
    	if (new_task == -1){
    		current_task = new_task;
    		continue;
    	}
    	if (new_task != current_task){
    		tasks[new_task].dispatches++;
    		current_task = new_task;
    	}

    	tasks[current_task].cpu_cycles += quantum;
        
        if (tasks[current_task].cpu_cycles >= tasks[current_task].length) {
            float quantum_fragment = tasks[current_task].cpu_cycles -
                tasks[current_task].length;
            tasks[current_task].cpu_cycles = tasks[current_task].length;
            tasks[current_task].finish_time = current_tick - quantum_fragment;
            
            ready[current_task] = (*set_state)(-1); //completed
            int next_task = (*get_next_task)(ready, ready_size, current_task);
            if (next_task != -1){
            	tasks[next_task].cpu_cycles += quantum_fragment;
            }
        }
    }
}

void run_simulation(int algorithm, int quantum)
{
    switch(algorithm) {
        case STRIDE:
            stride_scheduling(quantum);
            break;
        case PS:
            priority_scheduling();
            break;
        case RR:
            rr_scheduling(quantum);
            break;
        case FCFS:
        default:
            first_come_first_serve();
            break;
    }
}


void compute_and_print_stats()
{
    int tasks_at_level[PRIORITY_LEVELS] = {0,};
    float response_at_level[PRIORITY_LEVELS] = {0.0, };
    int scheduling_events = 0;
    int i;

    for (i = 0; i < num_tasks; i++) {
        tasks_at_level[tasks[i].priority]++;
        response_at_level[tasks[i].priority] += 
            tasks[i].finish_time - (tasks[i].arrival_time * 1.0);
        scheduling_events += tasks[i].dispatches;

        printf("Task %2d: cpu (%5.1f), response (%6.1f), wait (%6.1f), # dispatch (%5d)\n",
            i, tasks[i].length,
            tasks[i].finish_time - tasks[i].arrival_time,
            tasks[i].finish_time - tasks[i].arrival_time - tasks[i].cpu_cycles,
            tasks[i].dispatches);
            
    }

    printf("\n");

    if (num_tasks > 0) {
        for (i = 0; i < PRIORITY_LEVELS; i++) {
            if (tasks_at_level[i] == 0) {
                response_at_level[i] = 0.0;
            } else {
                response_at_level[i] /= tasks_at_level[i];
            }
            printf("Priority level %d: average response time (%4.1f)\n",
                i, response_at_level[i]);
        }
    }

    printf ("Total number of scheduling events: %d\n", scheduling_events);
}


int main(int argc, char *argv[])
{
    int i = 0;
    int algorithm = FCFS;
    int quantum = 1;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-q") == 0) {
            i++;
            quantum = atoi(argv[i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            i++;
            if (strcmp(argv[i], "FCFS") == 0) {
                algorithm = FCFS;
            } else if (strcmp(argv[i], "PS") == 0) {
                algorithm = PS;
            } else if (strcmp(argv[i], "RR") == 0) {
                algorithm = RR;
            } else if (strcmp(argv[i], "STRIDE") == 0) {
                algorithm = STRIDE;
            }
        }
    }
         
    read_task_data();

    if (num_tasks == 0) {
        fprintf(stderr,"%s: no tasks for the simulation\n", argv[0]);
        exit(1);
    }

    init_simulation_data(algorithm);
    run_simulation(algorithm, quantum);
    compute_and_print_stats();

    exit(0);
}
