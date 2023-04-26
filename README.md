# Garden Shed for scheduling

## Description

This is a very simple arduino scheduling library. It works by making a loops of tasks and checking (on a regular basis) whether they should be executed depending on the time they were given.
When a task is scheduled to run it blocks all other tasks as well as al other loops that are running there for tasks should be small and not have delays in them. (Although I do plan on changing this in the future).
The library also contains a note and assert function however I plan on extracting these into there own library

## Quick start
First in setup.
``` cpp
Scheduler s1;
Scheduler s2;
void setup()
{
    // only if you want to see output from the not and assert functoins
    Serial.begin(9600); 
    sched_push_task(&s1, []{note("hello s1:fun1");}, "fun1", 1000);
    sched_push_task(&s1, []{note("hello s1:fun2");}, "fun2", 1000);
    sched_push_task(&s2, []{note("hello s2:fun1");}, "fun1", 1000);
    sched_push_task(&s2, []{note("hello s2:fun2");}, "fun2", 1000);
}
```
The function id (third argument to sched_update) currently does nothing, it was for debugging and preparation for future features

Then in loop.
```cpp
void loop()
{
    sched_update(&s1);
    sched_update(&s2);
}
```
