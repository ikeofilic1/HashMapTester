#include <stdio.h>
#include "timer.h"

double getwalltime(), getcputime();

#ifdef _WIN32
double getwalltime()
{
    LARGE_INTEGER time, freq;
    #define fn(x) QueryPerformanceCounter(x)

    if (!fn(freq) || !fn(time))
        return (double)GetTickCount64()*1e-3;
    else
        return (double) time.QuadPart/freq.QuadPart;
}

double getcputime() {
    FILETIME a,b,c,d;
    while (GetProcessTimes(GetCurrentProcess(), &a,&b,&c,&d) == 0) {}

    return (double)(d.dwLowDateTime |
        ((unsigned long long)d.dwHighDateTime << 32))*0.0000001;
}
#else
double getwalltime() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return  (double)t.tv_sec + (double)t.tv_nsec*1e-9;
}

double getcputime() {
    return  (double) clock()/CLOCKS_PER_SEC;
}
#endif

struct timer new_timer(mytime_t t) {
    struct timer m = { 0, 0, t};
    return m;
}

int init_timer(timerptr p, mytime_t type)
{
    if (type != WALLTIME && type != CPUTIME){
        fprintf(stderr,
                "Invalid clock type. Only valid types are \"WALLTIME\" and \"CPUTIME\"\n");
        return -1;
    }

    p->type = type;
    p->begin = p->fin = 0;
    return 0;
}

int start_timer(timerptr p)
{
    if (p->type == WALLTIME) {
        p->begin = getwalltime();
    }
    else if (p->type == CPUTIME) p->begin = getcputime();
    else {
        fprintf(stderr, "Did you forget to initialize the timer?\n");
        return -1;
    }
    return 0;
}

int end_timer(timerptr p)
{
    if (p->type == WALLTIME) {
        p->fin = getwalltime();
    }
    else if (p->type == CPUTIME) p->fin = getcputime();
    else {
        fprintf(stderr, "Did you forget to initialize the timer?\n");
        return -1;
    }
    return 0;
}

/*
 * params: s => string to print before results are printed.
 *  Should be NULL for nothing
 *         p => timer ptr to print duration of
 * return: void
 */
void print_duration(char *s, timerptr p)
{
    if (s) {
        printf("%s\n", s);
    }
    double elapsed = p->fin-p->begin;
    printf("\n  **Elapsed time**\n");
    printf("%s    %.5f\n",
           (p->type==WALLTIME)?"wall(real)":"cpu(system)", elapsed);
}
