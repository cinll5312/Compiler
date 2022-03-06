/************************* procobj.h **********************/
#include <stdlib.h>
struct procobjTag
{
  char name[36];
  struct procobjTag *next;
  struct idobjTag *head, *tail;
} *procobjTop=NULL;

struct procobjTag * newProcobj(char *name)
{
  struct procobjTag *p=malloc(sizeof(struct procobjTag));
  strcpy(p->name, name);
  p->head = NULL;
  p->tail = NULL;
  p->next = NULL;
  return p;
}

void procpush(struct procobjTag *p)
{
  if (procobjTop != NULL)
    p->next = procobjTop;
  procobjTop = p;
}

struct procobjTag * procpop()
{
  struct procobjTag *p=procobjTop;
  procobjTop = procobjTop->next;
  return p;
}

struct procobjTag * findProcobj(char *procname)
{
  struct procobjTag *p;
  p = procobjTop;
  while (p!=NULL)
  {
    if (strcmp(p->name, procname)==0)
      return p;
    else
      p = p->next;
  }
  return p;
}

char *procobjToString()
{
  static char str[1024];
  struct procobjTag *p;
  struct idobjTag *v;
  p = procobjTop;
  strcpy(str, "");
  while (p!=NULL)
  {
    strcat(str, p->name);
    strcat(str, "\n");
    v=p->head;
    while (v!=NULL)
    {
      strcat(str, idobjToString(v));
      strcat(str, "\n");
      v = v->next;
    }
    p = p->next;
  }
  return str;
}

struct idobjTag *newIdobj(char name[], int sym, int attr,
                             int level, char procname[])
{
  struct idobjTag *p=malloc(sizeof(struct idobjTag));
  strcpy(p->name, name);
  p->sym = sym;
  p->attr = attr;
  p->level = level;
  strcpy(p->procname, procname);
  p->next = NULL;
  return p;
}

void varlistadd(struct procobjTag *p, struct idobjTag *v)
{
  v->next = NULL;   
  if (p->head == NULL)
  {
    p->head = v;
    p->tail = v;
  }
  else
  {
    (p->tail)->next = v;
    p->tail = v;
  }
}

struct idobjTag *getIdobj(struct procobjTag *p, char *id)
{
  struct idobjTag *v=NULL;
  while (p!=NULL)
  {
    v=p->head;
    while (v!=NULL)
    {
      if (strcmp(v->name,id)==0)
        return v;
      else
        v = v->next;
    }
    p = p->next;
  }
  v = NULL;
  return v;
}
