/* Instance generator for the General 0-1 Fractional Programming
   Implemented by Claudio N. Meneses
   Last modified: July 30, 2004.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

FILE *funcFileOpen(/* input parameters */
                   char sInstanceFile[])
{
  FILE *fp;

  fp = fopen(sInstanceFile,"w+");
  if (fp == NULL)
  {
    fprintf(stdout, "\nfuncFileOPen: file pointer is null");
    exit(1);
  } 
   return (fp);
} 

/* Generate random numbers using an uniform distribution function */
int funcUnif(long *lInstanceSeed,int iLow,int iHigh)
{
  long lm=LONG_MAX,lb=127773;
  int ia=16807,ic=2836;
  double dValue_0_1;

  *lInstanceSeed=ia*((*lInstanceSeed)%lb) - ((*lInstanceSeed)/lb)*ic;
  if (*lInstanceSeed < 0) *lInstanceSeed += lm;
  dValue_0_1 = ((double) *lInstanceSeed)/lm;
  return (iLow + (int)(dValue_0_1 * (iHigh-iLow+1)));
} 

void procGenerateInstance(/* input parameters */
                          int in,int im,int irhsknapsack,int LOW,int HIGH,
                          int iInstanceNumber,long *lInstanceSeed)
{
   FILE *pFilePointer;
   char sInstanceFile[20] = "",sString[20]="";
   int i,j,sum;
   int *ai,*bi,**aij,**bij;

   if (sprintf(sInstanceFile,"m%dn%dk%dl%dh%di%ds%ld.ins",im,in,irhsknapsack,LOW,HIGH,
                             iInstanceNumber,*lInstanceSeed) == EOF)
   {
       fprintf(stdout,"\nprocGenerateInstance: error in sprintf(sInstanceFile ...)");
       exit(1);
   } 
   pFilePointer = funcFileOpen(sInstanceFile);

   /* allocates memory to an array */
   if ((ai =(int *) malloc(sizeof(int) * im)) == NULL)
   {
     fprintf(stdout,"\nprocGenerateInstance: not enough memory to allocate ai");
     exit(1); 
   }
   if ((bi =(int *) malloc(sizeof(int) * im)) == NULL)
   {
     fprintf(stdout,"\nprocGenerateInstance: not enough memory to allocate bi");
     exit(1); 
   }
   if ((aij =(int **) malloc(sizeof(int) * im)) == NULL)
   {
     fprintf(stdout,"\nprocGenerateInstance: not enough memory to allocate aij");
     exit(1); 
   }
   for (i = 0; i < im; i++)
     if ((aij[i] =(int *) malloc(sizeof(int) * in)) == NULL)
     {
        fprintf(stdout,"\nprocGenerateInstance: not enough memory to allocate aij");
        exit(1); 
     }
   if ((bij =(int **) malloc(sizeof(int) * im)) == NULL)
   {
     fprintf(stdout,"\nprocGenerateInstance: not enough memory to allocate bij");
     exit(1); 
   }
   for (i = 0; i < im; i++)
     if ((bij[i] =(int *) malloc(sizeof(int) * in)) == NULL)
     {
        fprintf(stdout,"\nprocGenerateInstance: not enough memory to allocate bij");
        exit(1); 
     }
      
   for (i = 0; i < im; i++)
     ai[i] = funcUnif(&(*lInstanceSeed),LOW,HIGH);
   for (i = 0; i < im; i++)
     for (j = 0; j < in; j++)
       aij[i][j] = funcUnif(&(*lInstanceSeed),LOW,HIGH); 

   for (i = 0; i < im; i++)
   {
     sum = 0;
     for (j = 0; j < in; j++)
     {
       bij[i][j] = funcUnif(&(*lInstanceSeed),LOW,HIGH); 
       sum += bij[i][j];
     }
     bi[i] = funcUnif(&(*lInstanceSeed),abs(sum)+1,abs(sum)+HIGH);
   }

   /* write data to file */
   
   fputs("ai ",pFilePointer);
   for (i = 0; i < im; i++)
   {
     sprintf(sString,"%d ",ai[i]);     
     fputs(sString,pFilePointer);
   }
   fputs("\n",pFilePointer);
   fputs("bi ",pFilePointer);
   for (i = 0; i < im; i++)
   {
     sprintf(sString,"%d ",bi[i]);   
     fputs(sString,pFilePointer);  
   }
   fputs("\n",pFilePointer);
   fputs("aij\n",pFilePointer);
   for (i = 0; i < im; i++)
   {
     for (j = 0; j < in; j++)
     {
       sprintf(sString,"%d ",aij[i][j]);     
       fputs(sString,pFilePointer);
     }
     fputs("\n",pFilePointer);
   }
   fputs("bij\n",pFilePointer);
   for (i = 0; i < im; i++)
   {
     for (j = 0; j < in; j++)
     {
       sprintf(sString,"%d ",bij[i][j]);     
       fputs(sString,pFilePointer);
     }
     fputs("\n",pFilePointer);
   }
      
   fclose(pFilePointer);
} /* procGenerateInstance */

int main(int argc, char *argv[])
{
   int  im,in,irhsknapsack,LOW,HIGH,iInstanceNumber;
   long lInstanceSeed;

   if (argc != 8)
   {
      fprintf(stdout,"\n Usage: gifp m n k l h i s");
      fprintf(stdout,"\n m: number of quotients,");
      fprintf(stdout,"\n n: number of variables,");
      fprintf(stdout,"\n k: rhs knapsack,");
      fprintf(stdout,"\n l: low,");
      fprintf(stdout,"\n h: high, ");
      fprintf(stdout,"\n i: instance number,");
      fprintf(stdout,"\n s: generator seed (integer number).\n");
      exit(1);
   }  
   else
   {
      im = atoi(argv[1]);
      in = atoi(argv[2]);
      irhsknapsack = atoi(argv[3]);
      LOW = atoi(argv[4]);
      HIGH = atoi(argv[5]);
      iInstanceNumber = atoi(argv[6]);
      lInstanceSeed = atol(argv[7]);
   } 

   procGenerateInstance(in,im,irhsknapsack,LOW,HIGH,iInstanceNumber,&lInstanceSeed);
   return (0);
} /* main */
