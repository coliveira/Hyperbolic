/*----------------------------------------------------------
 * debug.c: useful functions for debugging programs 
 * 
 *  (c) 1999-2000, Carlos Alberto de S. Oliveira
 *----------------------------------------------------------*/

#ifndef NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "common.h"

typedef struct _mem_debug {
  struct _mem_debug *next;
  struct _mem_debug *prev;
} mem_debug;

static mem_debug head;
static unsigned long int debug_total_mem=0;
static unsigned long int max_size=0;

#define END_MEM_CHAR  0xC9
#define INIT_MEM_CHAR 0xD7
#define FREE_MEM_CHAR 0xB3

int verify_memory(char *p, char *file, int line)
{
  int *i, *end;
  if (!p) {
    printf("\n\nerror in %s line %d: null pointer.\n",
	   file, line);
    return 0;
  }

  i = (int *)p;
  --i;
  if (!(*i > 0 && ((unsigned int)*i) <= max_size)) {
    printf("\n\nerror in %s line %d: stored size is %d\n",
	   file, line, *i);
    return 0;
  }
  end = (int *)(p + *i);
  if ( *end != END_MEM_CHAR) {
    fprintf(stderr, "\n\nerror in %s line %d: end mark is wrong\n",
	    file, line);
    return 0;
  }
  return 1;
}


int verify_pointers(int what, char *p, char *file, int line)
{
  static int flag = 1;
  mem_debug *pt;
  if (flag) {
    flag = 0;
    head.next = head.prev = &head;
  }
  switch (what) {
  case INSERT_POINTER:
    pt = (mem_debug *)p;
    pt->prev = head.prev;
    pt->next = &head;
    head.prev->next = pt;
    head.prev = pt;
    break;
    
  case VERIFY_LIST:
    pt = &head;
    while (pt->next != &head) {
      if (!verify_memory((char *)(((int *)(pt->next+1))+1), file, line)) {
	printf("error in verify_pointers!\n");
	exit(1);
      }
      pt = pt->next;
    }
    break;

  case REMOVE_POINTER:
    pt = (mem_debug *)p;
    pt->prev->next = pt->next;
    pt->next->prev = pt->prev;
    break;

  default:
    printf("\n\nerror: wrong parameter to verify_pointers\n");
  }
  return 1;
}


#if 0
extern int hack_show;
#endif
void *mymalloc(unsigned int size, char *file, int line)
{
  char *p;
  int *i, *end;
  mem_debug *md;
  if (size <= 0){
    printf("\n\nerror in malloc, %s, line %d: size is %d\n",
	   file, line, size);
    exit(1);
  }  
  p = (char *)malloc(size+2*sizeof(int)+sizeof(mem_debug));
  if (!p) {
	  printf("error allocating memory in file %s line %d\n", file, line);
	  return 0;
  }
  md = (mem_debug *)p;
  verify_pointers(INSERT_POINTER, p, file, line);
  i = (int *)(md+1);
  debug_total_mem += size;
  *i = size;
  memset(i+1, INIT_MEM_CHAR, size);
  p = (char *)i;
  end = (int *)(p + size + sizeof(int));
  *end = END_MEM_CHAR;
  if (size > max_size)
    max_size = size;
#if 0
 	if (hack_show)  { 
	printf("%u %s line %d %u\n", 
   debug_total_mem, file, line, size); 
   fflush(stdout); } 
#endif
  return i+1;
}

 
void myfree(char *p, char *file, int line)
{
  int *i;
  mem_debug *md;
  
  if (!verify_memory(p, file, line)) {
    exit(1);
  }
  
  i = (int *)p;
  --i;
  md = (mem_debug *)i;
  md--;
  p = (char *)md;
  verify_pointers(REMOVE_POINTER, p, file, line);
  debug_total_mem -= *i;
#if 0
  if (hack_show) {
  printf("%u, %s line %d %u\n", debug_total_mem, file, line, *i);
  fflush(stdout); } 
#endif
  memset(p, FREE_MEM_CHAR, *i+2*sizeof(int)+sizeof(mem_debug));
  free(p);
}

void mymemset(char *p, char c, unsigned int size, char *file, int line)
{
  int *i;
  if (!verify_memory(p, file, line))
    exit(1);

  i = (int *)p;
  --i;
  if (((unsigned int)*i) < size){
    fprintf(stderr, "\n\nerror in %s, line %d: memset to a size"
	    " larger than available\n", file, line);
    exit(1);
  } else if (((unsigned int)*i) > size){
    fprintf(stderr, "\n\nwarning: in %s, line %d memset to a size"
	    " smaller than available\n", file, line);
  }
  memset(p, c, size);
}

#ifdef TESTMODULE
int main()
{
  int *p, i;
  p = NewVec(int, 30);
  for (i=0; i<30; i++)
    p[i] = 36+i;
  Delete(p);
  return 0;
}
#endif

#endif /* DEBUG */
