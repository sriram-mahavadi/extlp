/* 
 * File:   Timer.h
 * Author: Sriram Mahavadi
 *
 * Created on 21 March, 2014, 10:17 PM
 */

#ifndef TIMER_H
#define	TIMER_H
#include <string.h>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
using std::string;


#include <stdio.h>

class Timer {
public:
    struct timeval start, end;
    long mtime, seconds, useconds;

    

    void startTimer() {
        gettimeofday(&start, NULL);
    }

    void endTimer() {
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
        printf("Elapsed time: %ld milliseconds\n", mtime);
    }

    string getElapsedTimeInSeconds() {
        ostringstream mystream;
        mystream << seconds;
        return mystream.str();
    }

    string getElapsedTimeInMilliSeconds() {
        ostringstream mystream;
        mystream << mtime;
        return mystream.str();
    }

    unsigned int getCurrentTime() {
        time_t timer;
        struct tm y2k;
        double seconds;

        y2k.tm_hour = 0;
        y2k.tm_min = 0;
        y2k.tm_sec = 0;
        y2k.tm_year = 100;
        y2k.tm_mon = 0;
        y2k.tm_mday = 1;

        time(&timer); /* get current time; same as: timer = time(NULL)  */

        seconds = difftime(timer, mktime(&y2k));
        return seconds;
    }

    string displayElapsedTimeInMilliSeconds() {
        ostringstream mystream;
        mystream << mtime;
        cout << "\n#Elapsed Time : " << mystream.str() << " ms" << endl;
        return mystream.str();
    }


};

#endif	/* TIMER_H */

