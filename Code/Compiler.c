#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** 自訂表頭檔
*/
#include "scanner.h"
#include "resword.h"
#include "err.h"
#include "followsym.h"
#include "idobj.h"
#include "procobj.h"

/*
** 自訂常數
*/
#define IDLEN      36
#define BUFSIZE   512
#define PROCDEPTH  36


/*
** 自訂函式原型
*/
void Identifier();
void DoWhileStatement();
void Number();
void Factor();
void Term();
void Expression();
void Condition();
void WriteStatement();
void ReadStatement();
void WhileStatement();
void IfStatement();
void CompoundStatement();
void AssignmentStatement();
void Statement();
void ForStatement();
void VarDeclaration();
void Block();
void MainFunction();
void ProgramHead();
void Program();
/*
** 整體變數
*/
struct symbolTag *token;
int errorCount = 0;
struct idobjTag *idobj;
char outname[IDLEN];
FILE *outfile;
int labelCount = 0;
int level = -1;
char progname[IDLEN];
char procname[IDLEN];
struct procobjTag *procStack[BUFSIZE];
int procTop = 0;
struct procobjTag *procobj = NULL;
char id[IDLEN];
char buf[BUFSIZE];


/*
** Error()
*/
void Error(int n)
{
    int j;
    printf("****");
    for (j=0; j<=token->right; j++) printf(" ");
    printf("^%d  %s\n",n, errmsgs[n]);
    errorCount++;
}
/*
** checkexist()
*/
int checkexist()
{
    idobj=getIdobj(procStack[procTop-1], token->value);
    if (idobj==NULL)
      return 0;
    else
      return 1;
}
/*
** skip()
*/
void skip(char follows[], int n)
  {
    if (follows[token->sym]==0)
    {
        Error(n);
        while (follows[token->sym]==0)
            token = nextToken();
    }
}

/*
** 語法規則#1 <Program>
*/
void Program()
{
    if(strcmp(token->value,"#")==0) ProgramHead();
    MainFunction();
    //if(token->sym != symEOF) Error(0);
}

/*
** 語法規則#2 <ProgramHead>
*/
void ProgramHead()
{
     if (strcmp(token->value,"#")==0)
    {
      token = nextToken();
      if (strcmp(token->value,"include")==0)
      {
        token = nextToken();
        if (strcmp(token->value,"<")==0)
        {
			token = nextToken();
			if (strcmp(token->value,"stdio")==0)
          	{
				token = nextToken();  
				if (strcmp(token->value,".")==0)
          		{
					token = nextToken(); 
					if (strcmp(token->value,"h")==0)
          			{
						token = nextToken();  
						if (strcmp(token->value,">")==0)
          				{
							token = nextToken();  
						}
						else
	            		{
				        	Error(1);
				        	skip(statement,23);
						} 
					}
					else
            		{
			        	Error(1);
			        	skip(statement,23);
					} 
				}
				else
            	{
		        	Error(1);
		        	skip(statement,23);
				} 
			}
			else
            {
	        	Error(1);
	        	skip(statement,23);
			} 
		}
        else
        {
        	Error(1);
        	skip(statement,23);
		}  
      }
      else
      {
      	Error(1);
      	skip(statement,23);
	  }  
    }
    else
    {
    	Error(1);
    	skip(statement,23);
	}
      
}

/*
** 語法規則#3 <MainFunction>
*/
void MainFunction()
{
    struct procobjTag *p;
    if(strcmp(token->value,"int")==0)
	{
		strcpy(procname, token->value);
    	token = nextToken();
		 p = newProcobj(procname);
        p->next = NULL;
        p->head = NULL;
        p->tail = NULL;
        procpush(p);
        procStack[procTop++] = p;
        ++labelCount;
        sprintf(buf,
          ";************** %s ****************\n"
          ";\n"
          "\tORG\t100H\n"
          "\tJMP\t_start%d\n"
          "_intstr\tDB\t'     ','$'\n"
          "_buf\tTIMES 256 DB ' '\n"
          "\tDB 13,10,'$'\n",
          outname, labelCount);
        fprintf(outfile, buf);
        strcpy(buf, "%include\t\"dispstr.mac\"\n");
        strcat(buf, "%include\t\"itostr.mac\"\n");
        strcat(buf, "%include\t\"readstr.mac\"\n");
        strcat(buf, "%include\t\"strtoi.mac\"\n");
        strcat(buf, "%include\t\"newline.mac\"\n");
        fputs(buf, outfile);
        if(strcmp(token->value,"main")==0)
   	 	{
       		 token = nextToken();
    		if(token->sym == symLPAREN)
			{
				token = nextToken();
    			if(token->sym == symRPAREN)
				{
					token = nextToken();
					if(token->sym != symLBRAC) Error(10);
    				token = nextToken();
   			 		Block();
				}else
				{
					Error(18);
					skip(statement,23);
				}
			}else
			{
				Error(17);
				skip(statement,23);
			}
    	}else
    	{
    		Error(2);
    		skip(statement,23);
		}	
	}else
	{
		Error(2);
		skip(statement,23);
	}
    
    if(token->sym != symRBRAC) Error(11);
}

/*
** 語法規則#4 <Block>
*/  
void Block()
{
    if(strcmp(token->value,"int")==0) VarDeclaration();
    sprintf(buf, "_start%d:\n", labelCount);
    fprintf(outfile, buf);
    while(token->sym != symRBRAC)
    	Statement();
}

/*
** 語法規則#5 <VarDeclaration>
*/
void VarDeclaration()
{
    
    if (strcmp(token->value, "int")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        idobj=newIdobj(token->value,token->sym,
          symVAR, level, procStack[procTop-1]->name);
        varlistadd(procobjTop, idobj);
        sprintf(buf, "%s_%s\tDW\t",
          procStack[procTop-1]->name, token->value);
        fprintf(outfile, buf);
        token = nextToken();
      }
      if (token->sym == symBECOMES)
      {
        token = nextToken();
        if(token->sym == symNUMBER)
        {
            sprintf(buf, "%s\n", token->value);
            fprintf(outfile, buf);
            token = nextToken();
        }
        else
        {
            Error(22);
            skip(statement,23);
        }
      }
      else
      {
          sprintf(buf, "0\n");
          fprintf(outfile,buf);
      }
      while (token->sym == symCOMMA)
      {
          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            idobj=newIdobj(token->value,token->sym,
              symVAR, level, procStack[procTop-1]->name);
            varlistadd(procobjTop, idobj);
            sprintf(buf, "%s_%s\tDW\t",
              procStack[procTop-1]->name, token->value);
            fprintf(outfile, buf);
            token = nextToken();
          }
          if (token->sym == symBECOMES)
          {
            token = nextToken();
            if(token->sym == symNUMBER)
            {
                sprintf(buf, "%s\n", token->value);
                fprintf(outfile, buf);
                token = nextToken();
            }
            else
            {
                Error(22);
                skip(statement,23);
            }
          }
          else
          {
              sprintf(buf,"0\n");
              fprintf(outfile,buf);
          }
        }
        if(token->sym == symSEMI)
            token = nextToken();
        else
        {
            Error(6);
            skip(statement,23);
        }
    }
    else
    {
        Error(7);
        skip(statement,23);
        if(token->sym == symSEMI)token = nextToken();  
    }
}

/*
** 語法規則#6 <Statement>
*/
void Statement()
{
    //printf("%s\n",&token->value);
    if(isResword(token->value) != -1)
    {
        if(strcmp(token->value,"if")==0)
            IfStatement();
        else if(strcmp(token->value,"do")==0)
            DoWhileStatement();
        else if(strcmp(token->value,"while")==0)
            WhileStatement();
        else if(strcmp(token->value,"scanf")==0)
            ReadStatement();
        else if(strcmp(token->value,"printf")==0)
            WriteStatement();
        else if(strcmp(token->value,"for")==0)
            ForStatement();
        else if(strcmp(token->value, "int")==0)
            VarDeclaration();
    }
    else if(token->sym == symIDENTIFIER)
        AssignmentStatement();
    else
        skip(statement,23);

}

/*
** 語法規則#7 <AssignmentStatement>
*/
void AssignmentStatement()
{
    char result[BUFSIZE];
    idobj=getIdobj(procStack[procTop-1], token->value);
    if (idobj != NULL)
    {
      sprintf(result, "%s_%s",
        idobj->procname, token->value);
    }
    Identifier();
    if(token->sym == symBECOMES)
    {
        token = nextToken();
        Expression();
        sprintf(buf, "\tPOP\tAX\n"
                   "\tMOV\t[%s], AX\n", result);
        fprintf(outfile, buf);
        if(token->sym == symSEMI)
            token = nextToken();
        else if(token->sym == symRPAREN)
            return;
        else
        {
            Error(6);
            skip(statement,23);
        }
    }
    else
    {
        Error(8);
        skip(statement,23);
    }
    if(token->sym == symSEMI)token = nextToken(); 
}

/*
** 語法規則#8 <ifStatement>
*/
void IfStatement()
{
    int head, tail;
    if(strcmp(token->value,"if")==0)
    {
        token = nextToken();
        if(token->sym == symLPAREN)
        {
            token = nextToken();
            Condition();
            head = labelCount;
            tail = ++labelCount;
            sprintf(buf, "\tJMP\t_go%d\n"
                   "_go%d:\n", tail,head);
            fprintf(outfile, buf);
            if(token->sym == symRPAREN)
            {
                token = nextToken();
                CompoundStatement();
                sprintf(buf, "_go%d:\n", tail);
                fprintf(outfile, buf);
            }
            else
            {
            	Error(18);
            	skip(statement,23);
			}    
        }
        else
        {
        	Error(17);
        	skip(statement,23);
		} 
    }
    else
    {
    	Error(12);
    	skip(statement,23);
	}
}

/*
** 語法規則#9 <WhileStatement>
*/
void WhileStatement()
{
    int home, head, tail;
    if(strcmp(token->value,"while")==0)
    {
        token = nextToken();
        if(token->sym == symLPAREN)
        {
            token = nextToken();
            home = ++labelCount;
            sprintf(buf, "_go%d:\n", home);
            fprintf(outfile, buf);
            Condition();
            head = labelCount;
            tail = ++labelCount;
            sprintf(buf, "\tJMP\t_go%d\n"
                   "_go%d:\n", tail, head);
            fprintf(outfile, buf);
            if(token->sym == symRPAREN)
            {
                token = nextToken();
                CompoundStatement();
                sprintf(buf, "\tJMP\t_go%d\n", home);
                fprintf(outfile, buf);
            }
            else
            { 
				Error(18);
				skip(statement,23);
			}
               
        }
        else
        { 
			Error(17);
			skip(statement,23);
		}
    sprintf(buf, "_go%d:\n", tail);
    fprintf(outfile, buf);
    }
    else
    {
        Error(14);
        skip(statement,23);
    }
}

/*
** 語法規則#10 <DoWhileStatement>
*/
void DoWhileStatement()
{
    int head, tail;
    if(strcmp(token->value,"do")==0)
    {
        head = ++labelCount;
        labelCount--;
        token = nextToken();
        sprintf(buf,"_go%d:\n", head);
        fprintf(outfile,buf);
        CompoundStatement();
        if(strcmp(token->value,"while")==0)
        {
            token = nextToken();
            if(token->sym == symLPAREN)
            {
                token = nextToken();
                Condition();
                fprintf(outfile,"\n");
            }
            if(token->sym == symRPAREN)
            {
                token = nextToken();
                if(token->sym == symSEMI)
                    token = nextToken();
                else
                {
                	Error(6);
                	skip(statement,23);
				}
            }
            else
            {
                Error(18);
                skip(statement,23);
            }
        }
        else
        {
            Error(14);
            skip(statement,23);
        }
    }
    else
    {
        Error(15);
        skip(statement,23);
    }
    if(token->sym == symSEMI)token = nextToken();   
}

/*
** 語法規則#11 <ReadStatement>
*/
void ReadStatement()
{
    int i =0;
    char temp[100];
    if(strcmp(token->value,"scanf")==0)
    {
        token = nextToken();
        if(token->sym == symLPAREN)
        {
            token = nextToken();
            strcpy(temp,token->value);
            for( i = 0 ; i < 100 && temp[i]!='\0'; i += 2)
            {
                if(temp[i]=='%' && temp[i+1]=='d')
                {
                    break;
                }
            }
            token = nextToken();
            if(token->sym==symCOMMA)
            {
            	token = nextToken();
                if(token->sym == symPOINTER)
                {
                    token = nextToken();
                    idobj=getIdobj(procStack[procTop-1],token->value);
                    if (idobj != NULL)
                    {
                        sprintf(id,"%s_%s",idobj->procname,token->value);
                        sprintf(buf, "\treadstr\t_buf\n"
                                        "\tstrtoi\t_buf, '$', %s\n"
                                        "\tnewline\n", id);
                        fprintf(outfile, buf);
                    }
                    Identifier();
                }
                else
                {
                    Error(0);
                    skip(statement,23);
                }
			}
            if(token->sym == symRPAREN)
            {
                token = nextToken();
                if(token->sym == symSEMI)
                    token = nextToken();
                else
                {
                	Error(6);
                	skip(statement,23);
				}   
            }
            else
            {
                Error(18);
                skip(statement,23);
            }
        }
        else
        {
            Error(17);
            skip(statement,23);
        }
    }
    else
    {
        Error(3);
        skip(statement,23);
    }
    if(token->sym == symSEMI)token = nextToken();    
}

/*
** 語法規則#12 <WriteStatement>
*/
void WriteStatement()
{
    char num[100];
    int i;
    int newline = 0;
    if(strcmp(token->value,"printf")==0)
    {
        token = nextToken();
        if(token->sym == symLPAREN)
        {
            token = nextToken();
            if(token->sym == symSTRING)
            {
                strcpy(num,token->value);
                for(i = 0; i < 100 && num[i]!='\0'; i += 2)
                {
                	if(num[i]=='\\' && num[i+1]=='n')
                    {
                        newline = 1;
                        continue;
                    }
				}
                token = nextToken();
                if(token->sym == symCOMMA)
                {
                	token = nextToken();
                	if(token->sym == symPOINTER)token = nextToken();
	                else 
	                {
	                    Error(0);
	                    skip(statement,23);
					}
	                if(token->sym == symIDENTIFIER)
	                {
	                    sprintf(id, "%s_%s",
	                                    idobj->procname, token->value);
	                    sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
	                                    "\tMOV\tDX, _intstr\n"
	                                    "\tMOV\tAH, 09H\n"
	                                    "\tINT\t21H\n", id);
	                    fprintf(outfile, buf);
	                    if(newline = 1)
	                    {
	                    	sprintf(buf,"\tnewline\n");
                        	fprintf(outfile,buf);
						}
	                    Identifier();
	                }
	                else
	                {
	                    Error(21);
	                    skip(statement,23);
	                }
				}
                if(token->sym = symRPAREN)
                {
                    token = nextToken();
                    if(token->sym == symSEMI)token = nextToken();    
                    else
                    {
                    	Error(8);
                    	skip(statement,23);
					}   
                }
                else
                {
                    Error(18);
                    skip(statement,23);
                }
            }
        }
        else
        {
            Error(17);
            skip(statement,23);
        }
    }
    else
    {
        Error(19);
        skip(statement,23);
    }
    if(token->sym == symSEMI)token = nextToken();
}

/*
** 語法規則#13 <ForStatement>
*/

void ForStatement()
{
    int head, tail, add, home;
    if(strcmp(token->value,"for")==0)
    {
        token = nextToken();
        if(token->sym == symLPAREN)
        {
            token = nextToken();
            AssignmentStatement();
            head = ++labelCount;
            sprintf(buf,"_go%d:\n",head);
            fprintf(outfile,buf);
            Condition();
            home = labelCount;
            tail = ++labelCount;
            add = ++labelCount;
            sprintf(buf,"\tJMP\t_go%d\n",tail);
            fprintf(outfile,buf);
            if(token->sym == symSEMI)
            {
                token = nextToken();
                sprintf(buf,"_go%d:\n",add);
                fprintf(outfile,buf);
                AssignmentStatement();
                sprintf(buf,"\tJMP\t_go%d\n",head);
                fprintf(outfile,buf);
                if(token->sym == symRPAREN)
                {
                    token = nextToken();
                    sprintf(buf,"_go%d:\n",home);
                    fprintf(outfile,buf);
                    CompoundStatement();
                    sprintf(buf,"\tJMP\t_go%d\n"
                                "_go%d:\n",add,tail);
                    fprintf(outfile,buf);
                }
                else
                {
                    Error(18);
                    skip(statement,23);
                }
            }
            else
            {
                Error(6);
                skip(statement,23);
            }
        }
        else
        {
            Error(17);
            skip(statement,23);
        }
    }
    else
    {
        Error(13);
        skip(statement,23);
    }
    if(token->sym == symRBRAC) token = nextToken();     
}

/*
** 語法規則#14 <CompoundStatement>
*/
void CompoundStatement()
{
    if(token->sym == symLBRAC)
    {
        token = nextToken();
        while(token->sym == symIDENTIFIER)
            Statement();
        if(token->sym == symRBRAC)
            token = nextToken();
        else
        {
        	 Error(11);
        	skip(statement,23);
		}
           
    }
    else
    {
        Error(10);
        skip(statement,23);
        if(token->sym == symRBRAC || token->sym == symSEMI)token = nextToken();  
    }
}

/*
** 語法規則#15 <Condition>
*/
void Condition()
{
    Expression();
    if(token->sym == symLESS ||
       token->sym == symLEQ  ||
       token->sym == symEQ   ||
       token->sym == symNEQ  ||
       token->sym == symGREATER ||
       token->sym == symGEQ)
    {
        int op = token->sym;
        token = nextToken();
        Expression();
        sprintf(buf, "\tPOP\tBX\n"
                   "\tPOP\tAX\n"
                   "\tCMP\tAX, BX\n");
        fprintf(outfile, buf);
        switch (op)
        {
            case symEQ:
              sprintf(buf,"\tJE\t_go%d\n",(++labelCount));
              fprintf(outfile, buf);
              break;
            case symNEQ:
              sprintf(buf,"\tJNE\t_go%d\n",(++labelCount));
              fprintf(outfile, buf);
              break;
            case symLESS:
              sprintf(buf,"\tJL\t_go%d\n",(++labelCount));
              fprintf(outfile, buf);
              break;
            case symLEQ:
              sprintf(buf,"\tJLE\t_go%d\n",(++labelCount));
              fprintf(outfile, buf);
              break;
            case symGREATER:
              sprintf(buf,"\tJG\t_go%d\n",(++labelCount));
              fprintf(outfile, buf);
              break;
            case symGEQ:
              sprintf(buf,"\tJGE\t_go%d\n",(++labelCount));
              fprintf(outfile, buf);
              break;
        }
    }
    else
    {
        Error(20);
        skip(statement,23);
    }
}
  
/*
** 語法規則#16 <Expression>
*/
void Expression()
{
    if(token->sym == symPLUS || token->sym == symMINUS)
    {
        token = nextToken();
    }
    Term();
    while(token->sym == symPLUS || token->sym == symMINUS)
    {
        int op = token->sym;
        token = nextToken();
        Term();
        if(op==symPLUS)
        {
            fprintf(outfile,"\tPOP\tBX\n"
                    "\tPOP\tAX\n"
                    "\tADD\tAX, BX\n"
                    "\tPUSH\tAX\n");
        }
        else
        {
            fprintf(outfile, "\tPOP\tBX\n"
                     "\tPOP\tAX\n"
                     "\tSUB\tAX, BX\n"
                     "\tPUSH\tAX\n");
        }
    }
}

/*
** 語法規則#17 <Term>
*/
void Term()
{
    Factor();
    while(token->sym == symMUL || token->sym == symDIV)
    {
        int op = token->sym;
        token = nextToken();
        Factor();
        if(op==symMUL)
        {
            fprintf(outfile, "\tPOP\tBX\n"
                     "\tPOP\tAX\n"
                     "\tMUL\tBX\n"
                     "\tPUSH\tAX\n");
        }
        else
        {
            fprintf(outfile, "\tPOP\tBX\n"
                     "\tMOV\tDX, 0\n"
                     "\tPOP\tAX\n"
                     "\tDIV\tBX\n"
                     "\tPUSH\tAX\n");
        }
    }
}
/*
** 語法規則#18 <Factor>
*/
void Factor()
{
    if(token->sym == symIDENTIFIER)
    {
        idobj=getIdobj(procStack[procTop-1],token->value);
        if (idobj != NULL)
        {
            sprintf(id, "%s_%s",idobj->procname, token->value);
            sprintf(buf, "\tPUSH\tWORD [%s]\n", id);
            fprintf(outfile, buf);
        }
        Identifier();
    }
    else if(token->sym == symNUMBER)
    {
        sprintf(buf, "\tPUSH\t%s\n", token->value);
        fprintf(outfile, buf);
        Number();
    }
    else if(token->sym == symLPAREN)
    {
        token = nextToken();
        Expression();
        if(token->sym == symRPAREN)
        {
            token = nextToken();
        }
        else
        {
            Error(18);
            skip(statement,23);
        }
    }
}

/*
** 語法規則#19 <Identifier>
*/
void Identifier()
{
    if(token->sym == symIDENTIFIER)
    {
        idobj=getIdobj(procStack[procTop-1],token->value);
        if (idobj == NULL)
            Error(26);
        token = nextToken();
    }
    else
        Error(21);
}

/*
** 數字符記處理
*/
void Number()
{
    if(token->sym == symNUMBER)
    {
        token = nextToken();
    }
    else
        Error(22);
}
/*
************************** 主程式 **********************
*/
int main(int argc, char *argv[])
{
    int i;
    FILE *f=fopen(argv[1], "r");
    scanner(f);
    followsyminit();
    token = nextToken();
    outname[100]="test";
    for(i=0;i<100 && argv[1][i]!='.';i++)
    {
        outname[i]=argv[1][i];
    }
    outname[i]='\0';
    strcpy(progname, outname);
    sprintf(buf,"%s.asm",outname);
    outfile = fopen(buf,"w");
    Program();
    sprintf(buf,"\tMOV\tAX, 4C00H\n"
                "\tINT\t21H\n");
    fprintf(outfile,buf);
    printf("\n  Plone compile completed. "
      "\n  Error count : %d\n", errorCount);

    if (argc==3)
    {
      printf("\n程序結構堆疊內容如下：\n%s\n",
        procobjToString());
    }
    fclose(outfile);
    fclose(f);
    if (errorCount==0)
    {
      FILE *batchfile;
      sprintf(buf, "%s.bat", outname);
      batchfile=fopen(buf,"w");
      fprintf(batchfile, "nasm %s.asm -o %s.com\n",
              outname, outname);
      fprintf(batchfile, "%s.com\n", outname);
      fclose(batchfile);
     }
    return 0;
}
