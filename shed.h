#ifndef _SCHEDULE
#define _SCHEDULE

#include <Arduino.h>
#include <string.h>

#define TASK_LIMIT 15

typedef unsigned long ulong;
typedef void (*Procedure) ();

// tasks are linked list thus the reference to task in it's definition
struct Task {
    const char* id;
    Procedure proc;
    // when clock excedes this time proc must be executed
    ulong target_execution_time;
    ulong _interval;
    Task *nextTask;
};

struct TaskArena {
    Task task_buffer[TASK_LIMIT];
    bool task_open[TASK_LIMIT];
    const size_t cap = TASK_LIMIT;

    TaskArena();
    Task* new_block();
    void free_block(Task *block);
    void print() const;
};

struct Scheduler {
    Task *head = NULL;
    Task *tail = NULL;
    TaskArena arena;
};


void __assert_msg(const char* expr, const char *msg, const char *file, int line);
void __note(const char *msg, const char *file, int line);
#define assert(expr, msg) if(!(expr)){__assert_msg(# expr, msg, __FILE__, __LINE__);}
#define note(msg) __note(msg, __FILE__, __LINE__);

bool sched_push_task(Scheduler *sched, Procedure proc, const char* id, ulong interval); // interval states amount of time to wait
                                                                        // before next task (that is added) is executed
bool sched_update(Scheduler *sched);
bool sched_empty(const Scheduler &sched);
#endif // _SCHEDULE
