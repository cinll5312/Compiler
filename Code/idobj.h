/******************* idobj.h *****************/
struct idobjTag
{
  char name[36];
  int sym;
  int attr;
  int level;
  char procname[36];
  struct idobjTag *next;
};

char * idobjToString(struct idobjTag *p)
{
  static char str[512];
  sprintf(str,"  name:\"%s\"\tsym:%d\tattr:%d"
    "\tlevel:%d\tprocname:\"%s\"",
    p->name, p->sym, p->attr,
    p->level, p->procname);
  return str;
}
