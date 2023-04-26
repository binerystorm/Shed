#include "./shed.h"

void __note(const char *msg, const char *file, int line)
{
    Serial.print("NOTE: ");
    Serial.print(file);
    Serial.print(":");
    Serial.print(line);
    Serial.print(": ");
    Serial.println(msg);
}

void __assert_msg(const char* expr, const char *msg, const char *file, int line)
{
    Serial.println("Assertion failed:");
    Serial.print(file);
    Serial.print(":");
    Serial.print(line);
    Serial.print(": ");
    Serial.print(msg);
    Serial.println(", expretion:");
    Serial.println(expr);
    while(true);
}

TaskArena::TaskArena()
{
    memset(task_buffer, 0, sizeof(task_buffer));
    memset(task_open, true, sizeof(task_open));
}

Task *TaskArena::new_block()
{
    for(size_t i = 0; i < cap; i++){
        if (task_open[i]){
            task_open[i] = false;
            return &(task_buffer[i]);
        }
    }
    return NULL;
}

void TaskArena::free_block(Task *block)
{
    size_t idx = ((size_t) block - (size_t) task_buffer) / sizeof(Task);
    assert(!(task_open[idx]), "trying to free invalid block");
    task_open[idx] = true;
}

void TaskArena::print() const
{
    Serial.println(sizeof(task_open));
    for(size_t i = 0; i < cap; i++)
    {
        Serial.print(task_open[i]);
    }
    Serial.println("");
}

bool sched_push_task(Scheduler *sched, Procedure proc, const char* id, ulong interval)
{
    Task *task = sched->arena.new_block();
    assert(task != NULL, "arena full! or somthing has failed");
    task->proc = proc;
    task->id = id;
    // task->proc();
    task->target_execution_time = millis() + interval;
    task->_interval = interval;

    if(sched->head == NULL){
        assert(sched->tail == NULL, "push logic is incorrect")
        sched->head = task;
        sched->tail = task;
    }else{
        Task *old_task = sched->tail; 
        old_task->nextTask = task;
        sched->tail = task;
    }
    task->nextTask = sched->head;
    return true;
}

bool sched_update(Scheduler *sched)
{
    Task *task = sched->head;
    assert(task != NULL, "schedule is empty");
    ulong current_time = millis();
    if(current_time >= task->target_execution_time){
        Serial.println(task->id);
        task->proc();
        sched->tail = task;
        sched->head = task->nextTask;
        assert(sched->head != NULL, "in pushing of task nextTask was allowed to be null")
        sched->head->target_execution_time = current_time + sched->head->_interval;
        //sched->arena.free_block(task);
        return true;
    }
    return false;
}

bool sched_empty(const Scheduler &sched)
{
    bool ret;
    if(ret = sched.head == NULL)
        assert(sched.tail == NULL, "something went wrong in empting");
    return ret;
}

// Delay delay_init(ulong target)
// {
//     return {
//         .root = 0,
//         .elapsed = millis(),
//         .target = target,
//     };
// }
// 
// void delay_reset(Delay *d)
// {
//     d->root = millis();
//     d->elapsed = 0;
// }
// 
// bool delay_update(Delay *d)
// {
//     ulong current_time = millis();
//     d->elapsed = current_time - d->root;
//     if (d->elapsed >= d->target) {
//         delay_reset(d);
//         return true;
//     }
//     return false;
// }
// 
// void delay_print(const Delay &d)
// {
//     Serial.print("target: ");
//     Serial.println(d.target);
//     Serial.print("elapsed: ");
//     Serial.println(d.elapsed);
//     Serial.print("root: ");
//     Serial.println(d.root);
// }
