#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <windows.h>

class Stopwatch
{
 
 private:
     
    LARGE_INTEGER frequency;
    LARGE_INTEGER prevTime, currTime;
    double LIToSecs( LARGE_INTEGER & L) ;
    UINT     wTimerRes;

 public:
     Stopwatch() ;
     ~Stopwatch();
     void StartTimer();
     double GetElapsedTime(); // get time in seconds since the last call to this function
     void WaitUntil(long long time);  // wait until this time in milliseconds has passed
 };


#endif // STOPWATCH_H