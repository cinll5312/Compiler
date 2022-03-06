/********************** resword.h ******************/
#include <stdlib.h>
#define RESWORDMAX 9
char reswords[RESWORDMAX][10] =
  {
  "if","scanf","while","int"
  ,"main","return","do","for","printf"
  };
int isResword(char *s)
{
  int i;
  for (i=0; i<RESWORDMAX; i++)
  {
    if (strcmp(s, reswords[i])==0) return i;
  }
  return -1;
}
