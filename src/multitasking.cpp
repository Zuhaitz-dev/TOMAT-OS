#include <multitasking.h>

using namespace myos;
using namespace myos::common;

// In the future it might be ideal to work on this again. For different security levels
// Not my problem right now, so f*ck it tbh.
Task::Task(GlobalDescriptorTable *gdt, void entrypoing())
{
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;

    // cpustate -> gs = 0;
    // cpustate -> fs = 0;
    // cpustate -> es = 0;
    // cpustate -> ds = 0;

    // cpustate -> error = 0;

    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoing;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;   | > Who was gonna say I was gonna comment the ss in a German tutorial xd
    //                      | > Now, seriously, security levels again.
    cpustate -> eflags = 0x202;

}



Task::~Task()
{

}


TaskManager::TaskManager()
{
    numTasks = 0;       // No tasks at the beginning
    currentTask = -1;
}



TaskManager::~TaskManager()
{

}



bool 
TaskManager::AddTask(Task* task)
{
    if (numTasks >= 256)
        return false;
    
    tasks[numTasks++] = task;
    
    return true;
}



CPUState*
TaskManager::Schedule(CPUState* cpustate)
{
    if (numTasks <= 0)
        return cpustate;

    if (currentTask >= 0)
        tasks[currentTask] -> cpustate = cpustate;

    // Important lesson: increment isn't a thing, but pre-increment and post-increment are
    if (++currentTask >= numTasks)
        currentTask %= numTasks;
    
    return tasks[currentTask] -> cpustate;
}