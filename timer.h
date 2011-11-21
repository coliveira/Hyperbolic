#ifndef _TIMER_H
#define _TIMER_H

/* return the number of seconds since last call to set_initial_time  */
double get_time();

/* set the time of the begining of the program */
void set_initial_time();

/* returns the number of seconds since the begin of the program 
 * as a  double */
double timer();

#endif
