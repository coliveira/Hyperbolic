/*----------------------------------------------------------------
 *  common.h - Definitions common to all my C projects
 *
 *  (C)1996-2002, Carlos Alberto de S. Oliveira
 *----------------------------------------------------------------*/

#ifndef _COMMON_H
#define _COMMON_H

/*
 * "safe" versions of New, NewVec and Delete
 */
#define NewS(var, type, errlabel) \
  do { var = New(type); if (!var) goto errlabel; } while (0)

#define NewVecS(var, type, size, errlabel) \
  do { var = NewVec(type, size); if (!var) goto errlabel; } while (0)

#define CreateS(var, instr, errlabel) \
  do { var = instr; if (!var) goto errlabel; } while (0)

#define DeleteS(Object) \
  if (Object) Delete(Object)

#define Resize(a, t, s)  Realloc(a,t,s)


/*
 * Macros for memory allocation
 */
#ifndef NDEBUG
 enum {
   INSERT_POINTER,
   VERIFY_LIST,
   REMOVE_POINTER
 } ;

 void *mymalloc(unsigned int n, char *f, int l);
 void myfree(char *p, char *f, int l);
 void mymemset(char *p, char c, unsigned int size, char *file, int line);
 int myrealloc(void **a, int size, char *file, int line);
 int verify_pointers(int what, char *p, char *file, int line);
     
 #define New(type) \
	 ((type *)mymalloc(sizeof(type), __FILE__, __LINE__))
 #define NewVec(type,n)	\
         ((type *)mymalloc(sizeof(type)*(n), __FILE__, __LINE__))
 #define Delete(var) \
	 do { myfree(((char*)var), __FILE__, __LINE__); var = 0; } while (0)
 #define MemSet(var,c,len) \
         mymemset(((char*)var), (int)c, (len)*sizeof(*(var)),__FILE__, __LINE__)
 #define Realloc(a, t,s) myrealloc((void **)&a, sizeof(t)*(s), __FILE__,__LINE__)
 #define VERIFY_MEMORY verify_pointers(VERIFY_LIST, 0, __FILE__, __LINE__)
#else
 #define New(type)          ((type *)malloc(sizeof(type)))
 #define NewVec(type,n)	    ((type *)malloc(sizeof(type)*(n)))
 #define Delete(var)	    do { free(var); var = 0; } while (0)
 #define MemSet(var,c,len)  memset(var, (int)c, (len)*sizeof(*(var)))
 #define Realloc(a, t,s)    realloc((void *)&a, sizeof(t)*(s))
 #define VERIFY_MEMORY      do { ; } while (0)
#endif

#define NewVec2(var,type,m,n) do {                  \
  int ___m_ = (m), ___n_ = (n);                     \
  int __cont;                                       \
  if ((var = NewVec(type*,___m_)))                  \
    for (__cont = 0; __cont < ___m_; __cont++){     \
      var[__cont] = NewVec(type,___n_);             \
      if (!var[__cont]){                            \
	   for (__cont--; __cont>=0; __cont--)      \
	     Delete(var[__cont]);                   \
	 Delete(var); \
	 var = 0;     \
         break;       \
      }               \
    }                 \
} while (0)

#define NewVec2zero(var,type,m,n) do {                 \
  int ___m_ = (m), ___n_ = (n);                     \
  int __cont;                                       \
  if ((var = NewVec(type*,___m_)))                  \
    for (__cont = 0; __cont < ___m_; __cont++){     \
      var[__cont] = NewVec(type,___n_);             \
      memset(var[__cont], 0, sizeof(type)*___n_);   \
      if (!var[__cont]){                            \
	   for (__cont--; __cont>=0; __cont--)      \
	     Delete(var[__cont]);                   \
	 Delete(var); \
	 var = 0;     \
         break;       \
      }               \
    }                 \
} while (0)

#define NewVec2S(var,type,m,n,errlabel) do { \
  NewVec2(var,type,m,n); if (!var) goto errlabel; } while (0)

#define NewVec2Szero(var,type,m,n,errlabel) do { \
  NewVec2zero(var,type,m,n); if (!var) goto errlabel; } while (0)


#define DelVec2(var,m) do {                      \
  int __cont;                                    \
  if (var){                                      \
    for (__cont = (m)-1; __cont >= 0; __cont--)  \
      Delete(var[__cont]);  \
    Delete(var);            \
  }                         \
} while (0)
				           
/* some debug macros */
#ifndef NDEBUG
# define DBG(x) x
#else
# define DBG(x) /* empty */
#endif

#ifndef NDEBUG
#  define P(x)  (printf(" %d ",x), fflush(stdout));
#  define DS(X)	(printf(" %s ",X), fflush(stdout));
#  define DF(X)	(printf(" %f ",X), fflush(stdout));
#else  /* NDEBUG */
#  define P(X)     /* empty */
#  define DS(X)    /* empty */
#  define DF(X)    /* empty */
#endif /* NDEBUG */

#define clone_string(dest, st) do { \
  int __n = strlen(st);          \
  dest = NewVec(char, __n + 1);  \
  if (dest) strcpy(dest, st);    \
} while (0)

#define clone_stringS(dest, st, e)  do { \
  clone_string(dest, st); \
  if (!dest)  goto e;     \
} while (0)


/* simple random value */
#define random_int(x) (rand() % (x))
/* big random value */
#define long_rand(x)  \
((x == 0) ? 0 : ((abs(rand() << 16) | (rand())) % x))
/* exchange a and b */
#define swap(type, a, b) \
  do { type t = a; a = b; b = t; } while (0)

#endif /* _COMMON_H */
