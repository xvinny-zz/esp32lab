/** This test demonstrates interaction between three simple tasks via StatusRequest object.
 *  Task T1 runs every 5 seconds and signals completion of a status request st.
 *  Tasks T2 and T3 are waiting on the same request (st)
 *  Task T3 does not renew its interest in status request st, so it is only invoked once (first iteration)
 *  Task T2 is invoked every time st completes, because it renews its interest in status of status request object st every iteration of T1
 */

#define _TASK_SLEEP_ON_IDLE_RUN
#define _TASK_STATUS_REQUEST
#include <TaskScheduler.h>

StatusRequest statusRequest;
Scheduler scheduler;

// Callback methods prototypes
void Callback1();
void Disable1();
void Callback2();
void Callback3();
void PrepareStatus();

// Tasks
// (unsigned long aInterval=0, long aIterations=0, TaskCallback aCallback=NULL, Scheduler* aScheduler=NULL, bool aEnable=false, TaskOnEnable aOnEnable=NULL, TaskOnDisable aOnDisable=NULL);
#define T1_INTERVAL             5000
#define T1_ITERATIONS           TASK_ONCE
#define T1_CALLBACK             Callback1
#define T1_AENABLE              true
#define T1_ONENABLE_CALLBACK     NULL
#define T1_ONDISABLE_CALLBACK   Disable1

Task t1(T1_INTERVAL, TASK_ONCE, &T1_CALLBACK, &scheduler, T1_AENABLE, T1_ONENABLE_CALLBACK, &T1_ONDISABLE_CALLBACK);
Task t2(&Callback2, &scheduler);
Task t3(&Callback3, &scheduler);

/** T1 callback
 *  T1 just signals completion of statusRequest every 5 seconds
 */
void Callback1()
{
    Serial.println("T1: Signaling completion of statusRequest");
    statusRequest.signalComplete();
}

/** T1 On Disable callback
 *  This callback renews the status request and restarts T1 delayed to run again in 5 seconds
 */
void Disable1()
{
    Serial.println("Renewing the status request and restarting T1 delayed to run again in 5 seconds");
    PrepareStatus();
    t1.restartDelayed();
}

/** T2 callback
 *  Invoked when status request statusRequest completes
 */
void Callback2()
{
    Serial.println("T2: Invoked due to completion of statusRequest");
}

/** T3 callback
 *  Invoked when status request statusRequest completes.
 *  This is only run once since T3 does not renew its interest in the status request statusRequest after first iteration
 */
void Callback3()
{
    Serial.println("T3: Invoked due to completion of statusRequest");
}

/** Prepare Status request statusRequest for another iteration
 *  
 */
void PrepareStatus()
{
    Serial.println("Preparing status request");
    statusRequest.setWaiting(); // set the statusrequest object for waiting
    t2.waitFor(&statusRequest); // request tasks 1 & 2 to wait on the object statusRequest
}

/** Main Arduino code
 *  Not much to do here. Just init Serial and set the initial status request
 */
void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("TaskScheduler: Status Request Test 1. Simple Test.");

    scheduler.startNow();
    PrepareStatus();
    t3.waitFor(&statusRequest);

    Serial.println("Delaying t1");
    t1.delay();
}

void loop()
{
    scheduler.execute();
}