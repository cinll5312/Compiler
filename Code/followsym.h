/*************** followsym.h ***************/
#include "sym.h"
char factor[symSYMMAX];
char term[symSYMMAX];
char expression[symSYMMAX];
char condition[symSYMMAX];
char statement[symSYMMAX];
char block[symSYMMAX];
void followsyminit()
{
  factor[symSEMI] = 1;
  factor[symRPAREN] = 1;
  factor[symPLUS] = 1;
  factor[symMINUS] = 1;
  factor[symMUL] = 1;
  factor[symDIV] = 1;
  term[symSEMI] = 1;
  term[symRPAREN] = 1;
  term[symPLUS] = 1;
  term[symMINUS] = 1;
  expression[symSEMI] = 1;
  expression[symRPAREN] = 1;
  expression[symRBRAC] = 1;
  condition[symRPAREN] = 1;
  statement[symSEMI] = 1;
  statement[symRBRAC]=1;
  block[symSEMI] = 1;
}
