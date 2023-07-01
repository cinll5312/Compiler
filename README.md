# Compiler_C
一. 支援語法
```
1.<Program> ::= [<ProgramHead>]<MainFunction>
2. <ProgramHead> ::= #include<stdio.h>
3. <MainFunction> ::= ‘int main() {‘ <Block> ;‘}’
4. <Block> ::=[<VarDeclaration>] {<Statement>}
5. <VarDeclaration> ::= int <Identifier>[=<Number>]{, <Identifier>[=<Number>]} ;
6. <Statement> ::= 
[<VarDeclaration>|<WhileStatement>|<DoWhileStatement>|<ForStatement>|<AssignmentStatement
>|<WriteStatement>|<ReadStatement>]
7. <AssignmentStatement> ::=<Identifier> = <Expression>;
8. <IfStatement> ::= if (<Condition>){ <CompoundStatement>}
9. <WhileStatement> ::=while(<Condition>) <CompoundStatement>
10. <DoWhileStatement> ::=do <CompoundStatement> while(<Condition>);
11. <ReadStatement> ::= scanf ("%d " ,&<Identifier>);
12. <WriteStatement> ::= printf ("%d|%d\n" ,&<Identifier>);
13. <CompoundStatement> ::= ‘{‘ {<Statement>} ‘}’
14. <Condition> ::= <Expression> \<|<=|==|!=|>|>=\<Expression>
15. <Expression> ::= [+|-] <Term> { \+|-\<Term> }
16. <Term> :: = <Factor> { \*|/ \<Factor> }
17. <Factor> ::= <Identifier>|<Number>| (<Expression>)
18. <Identifier> ::= <Alpha> { <Alpha> | <Digit> }
19. <Number> ::= <Digit> { <Digit> }
20.<Alpha> ::=A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s
|t|u|v|w|x|y|z
21. <Digit> ::= 0|1|2|3|4|5|6|7|8|9
22. <String> ::= " 任何非雙引號的字元集合"
```
