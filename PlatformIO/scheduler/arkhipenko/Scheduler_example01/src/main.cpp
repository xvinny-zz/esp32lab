/** 
 *  TaskScheduler Test
 *
 *  Initially only tasks 1 and 2 are enabled
 *  Task1 runs every 2 seconds 10 times and then stops
 *  Task2 runs every 3 seconds indefinitely
 *  Task1 enables Task3 at its first run
 *  Task3 run every 5 seconds
 *  Task1 disables Task3 on its last iteration and changed Task2 to run every 1/2 seconds
 *  At the end Task2 is the only task running every 1/2 seconds
 */

#include <TaskScheduler.h>

// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();

#define T1_INTERVAL_MS 2000
#define T1_ITERATIONS 10

#define T2_INTERVAL_MS 3000
#define T2_ITERATIONS TASK_FOREVER

#define T3_INTERVAL_MS 5000
#define T3_ITERATIONS TASK_FOREVER

//Tasks
Task t4();
Task t1(T1_INTERVAL_MS, T1_ITERATIONS, &t1Callback);
Task t2(T2_INTERVAL_MS, T2_ITERATIONS, &t2Callback);
Task t3(T3_INTERVAL_MS, T3_ITERATIONS, &t3Callback);

Scheduler scheduler;

void t1Callback()
{
    Serial.print("t1: ");
    Serial.println(millis());

    if (t1.isFirstIteration())
    {
        scheduler.addTask(t3);
        t3.enable();
        Serial.println("t1: enabled t3 and added to the chain");
    }

    if (t1.isLastIteration())
    {
        t3.disable();
        scheduler.deleteTask(t3);
        t2.setInterval(500);
        Serial.println("t1: disable t3 and delete it from the chain. t2 interval set to 500");
    }
}

void t2Callback()
{
    Serial.print("t2: ");
    Serial.println(millis());
}

void t3Callback()
{
    Serial.print("t3: ");
    Serial.println(millis());
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Scheduler TEST");

    scheduler.init();
    Serial.println("Initialized scheduler");

    scheduler.addTask(t1);
    Serial.println("added t1");

    scheduler.addTask(t2);
    Serial.println("added t2");

    Serial.println("Waiting 5s to start...");
    delay(5000);

    t1.enable();
    Serial.println("Enabled t1");
    t2.enable();
    Serial.println("Enabled t2");
}

void loop()
{
    scheduler.execute();
}