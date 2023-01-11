// Copyright (c) 2022.
// Ikechukwuka Ofili

#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <sysinfoapi.h>
#else
#include <time.h>
#endif

//TODO: threading
typedef enum time_type {
    WALLTIME = 1,
    CPUTIME = 2,
    //THREAD = 4,
} mytime_t;

struct timer
{
    double begin, fin;
    mytime_t type;
};

struct timer;
typedef struct timer* timerptr;

int init_timer(timerptr timer, mytime_t time_type);
int start_timer(timerptr timer);
int end_timer(timerptr timer);
void print_duration(char *message, timerptr timer);
