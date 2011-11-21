/*----------------------------------------------------------
 *  parsecmdl.c
 *  Generic command line parsing utility.
 * 
 *  (c) 1996-2000, Carlos Alberto de S. Oliveira
 *----------------------------------------------------------*/

#ifndef _parsecml_h
#define _parsecml_h

/* possible types of options in command line                */
enum {
  CLT_INT,  /* there's a int to be stored in var     */
  CLT_STR,  /* there's a int to be stored in var     */
  CLT_OPT   /* It's only a option to be set true     */
};

#define fill_cmdl_st(p, my_patt, my_var, my_type, my_next) do { \
  strcpy((p)->pattern, my_patt);  \
 (p)->var = ((void *)my_var); \
 (p)->type = my_type;  \
 (p)->next = my_next; } while (0)


typedef struct _cmdl_st {
  char *help;
  char *pattern;              /* option in command line     */
  void *var;                  /* add to var we'll set       */
  int type;                   /* what will be stored in var */
  struct _cmdl_st *next;
} cmdl_st;

int cmdl_init(cmdl_st *cml, int n);
int cmdl_process(int argc, char **argv, cmdl_st *opt);
void cmdl_print_options(cmdl_st *opt);

#endif /* _parsecml_h */
