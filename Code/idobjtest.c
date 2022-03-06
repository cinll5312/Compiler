/******************** idobjtest.c *******************/
#include <stdio.h>
#include <stdlib.h>
#include "sym.h"
#include "idobj.h"
int main()
{
  struct idobjTag *p=malloc(sizeof(struct idobjTag));
  strcpy(p->name, "speed");
  p->sym = symIDENTIFIER;
  p->attr = symVAR;
  p->level = 1;
  strcpy(p->procname, "myproc");
  p->next = NULL;
  printf("idobjToString(p)=\n%s\n", idobjToString(p));
  return 0;
}
