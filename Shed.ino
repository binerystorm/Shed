#include "./shed.h"   

void lightOn()
{
    note("light on");
}

void lightOff()
{
    note("light off");
}

void foo()
{
    note("foo");
}

Scheduler sched;
bool added_foo = false;
int count = 0;
void setup()
{
    Serial.begin(9600);
    sched_push_task(&sched, lightOn, "lighon", 1000);
    sched_push_task(&sched, lightOff, "lightoff", 1000);
}

void loop()
{
    if(count == 5 && !added_foo){
        sched_push_task(&sched, foo, "foo", 500);
        added_foo = true;
    }
    if(sched_update(&sched)){
        count++;
        sched.arena.print();
    }
    //Serial.println(count);
    // if(sched_empty(sched)){
    //     sched_push_task(&sched, lightOn, "lighon", 1000);
    //     sched_push_task(&sched, lightOff, "lightoff", 1000);
    // }
}
