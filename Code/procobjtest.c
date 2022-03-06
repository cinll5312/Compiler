/********************* procobjtest.c *********************/
#include <stdio.h>
#include "idobj.h"
#include "procobj.h"
int main()
{
  struct procobjTag *A=newProcobj("A");
  struct procobjTag *B=newProcobj("B");
  struct idobjTag *id_1 = newIdobj("m",0,0,0,"A");
  struct idobjTag *id_2 = newIdobj("n",0,0,0,"A");
  struct idobjTag *id_3 = newIdobj("n",0,0,1,"B");
  struct idobjTag *id_4 = newIdobj("k",0,0,1,"B");
  varlistadd(A, id_1);
  varlistadd(A, id_2);
  varlistadd(B, id_3);
  varlistadd(B, id_4);
  procpush(A);
  procpush(B);
  printf("程序結構如下：\n%s\n", procobjToString());
  struct idobjTag *id = getIdobj(A,"n");
  printf("程序A裡的識別字n如下：\n%s\n",idobjToString(id));
  id = getIdobj(B,"n");
  printf("程序B裡的識別字n如下：\n%s\n",idobjToString(id));
  return 0;
}
