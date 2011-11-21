/*--------------------------------------------------------------------
 * PRQAP: A GRASP with Path Relinking to solve the 
 * Quadratic Assignment Problem.
 * by Carlos A.S. Oliveira*, Mauricio G.C. Resende, Panos M.Pardalos,
 *    (*) ISE dept., University of Florida, 2002
 *-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <math.h>
#include <unistd.h>

/*-------------------------------------------------------
 * Time-related functions 
 *------------------------------------------------------*/
double begin_time, total_time;

/* returns the number of seconds since the begin of the program 
 * as a  double */
double timer ()
{
   struct tms buff;  /* times buffer */
   if (times (&buff) == -1) {
      printf("call to times failed"); 
      exit(0);
   }
   return ((buff.tms_utime + buff.tms_stime) /(double)sysconf(_SC_CLK_TCK));
}

/* set the time of the begining of the program */
void set_initial_time()
{
   begin_time = timer();
}

/* return the number of seconds since last call to set_initial_time  */
double get_time()
{
   return (timer() - begin_time);
}

