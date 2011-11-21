/*----------------------------------------------------------
 *  parsecmdl.c
 *  Generic command line parsing utility.
 * 
 *  (c) 1996-2000, Carlos Alberto de S. Oliveira
 *----------------------------------------------------------*/

/** @name Command line parser.
 *
 * This module implements a command line parser. The objective
 * is encapsulate operations related to command line parsing.
 * The only interface of this module to the world is the
 * function `process_cmdl'. To call this function one needs to
 * fill a structure `cmdl_st'.
 */
/*@{*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "parsecml.h"

int cmdl_init(cmdl_st *cml, int n)
{
	int i;
   for (i=0; i<n-1; i++)
      cml[i].next = &cml[i+1];
   cml[i].next = 0;
	return 1;
}

/** @name find_opt
 * this function is responsible for find the elements defined
 * in the cmdl structure in the comand line vector.
 * @param arg pointer to the command line vector
 * @param opt command line options stored in the cmdl structure.
 */
cmdl_st *find_opt(char *arg, cmdl_st *opt)
{
  cmdl_st *p;
  for (p = opt; p; p = p->next)
    if (!strcmp(arg, p->pattern))
      return p;
  return 0;
}

/** returns true if c is a number */
int is_num(char *c)
{
   while (*c){
      if (!isdigit(*c)) {
	 return 0;
      }
      c++;
   }
   return 1;
}


/** @name process_this_arg
 * function used to process a specific arg and find of what type is it.
 * @param argc number of arguments in the command line.
 * @param argv vector of arguments.
 * @param ip integer pointing to the current option.
 * @param opt structure with possible command line options.
 */
static int process_this_arg(int argc, char **argv, int *ip,
			    cmdl_st *opt)
{
  cmdl_st *this_opt;
  int i = *ip;
  this_opt = find_opt(argv[i], opt);
  if (this_opt == 0) {   /* no option was recognized */
	  if (argv[i][0] == '-'){
		  printf("Unknow option %s\n", argv[i]);
		  return 0;
	  }
	  return 1;
  }
  switch (this_opt->type){
	  case CLT_OPT:
		  *((int*) this_opt->var) = 1;
		  return 1;

	  case CLT_INT:
		  if (argc-1<=i){  /* no more input */
			  fprintf(stderr, "Error: No argument for parameter %s\n", argv[i]);
			  return 0;
		  }
		  if (!is_num(argv[i+1])) {
			  printf("Argument to parameter %s is not an int.\n", argv[i]);
			  return 0;
		  }
		  *((int*) this_opt->var) = atoi(argv[i+1]);
		  *ip = i+1;
		  return 1;

  case CLT_STR:
    if (argc-1<=i){  /* no more input */
      printf("No argument for parameter %s\n", argv[i]);
      return 0;
    }
    strcpy((char *) this_opt->var, argv[i+1]);
    *ip = i+1;
    return 1;

  default:
    fprintf(stderr, "invalid type for command line option\n");
  }
  return 0;
}

/** @name print option
 * used to print the options present in the opt structure */
void cmdl_print_options(cmdl_st *opt)
{
  for (; opt; opt=opt->next)
    printf("    %s -- %s\n", opt->pattern, opt->help);
}

/** @name process cmdl
 * main interface of this module. It does the processing of
 * command line options.
 */
int cmdl_process(int argc, char **argv, cmdl_st *opt)
{
  int i;
  for (i=1; i<argc; i++)
    if (!process_this_arg(argc, argv, &i, opt))
      return 0;
  return 1;
}
/*@}*/
