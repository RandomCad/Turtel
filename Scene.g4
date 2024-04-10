grammar Scene;

// Parser rules
expr  : ( klamKon | number) '^' (klamKon | number) #Exp
      | ( klamKon | number) '*' (klamKon | number) #Mult
      | ( klamKon | number) '/' (klamKon | number) #Dife
      | expr '+' expr #Add
      | expr '-' expr #Dim
      | '|' expr '|'  #ABS
      | '-' number    #Negate
      | number	      #NumExpr	
      | var	      #VarExp
      ;
klamKon	: '(' expr ')' ;
number: Num     
      | Float   #Float
      ; 
var   : ID      # Variable
      | IncID   # GlobalVariable
      ;

// Lexer rules
Num   : [0-9]+ ;
Float : [0-9]+ '.' [0-9]+
      | '.' [0-9]+ ;

ID    : [_a-zA-Z0-9] [_@a-zA-Z0-9]* ;
IncID : '@' [_@a-zA-Z0-9]* ;

t: .*;

WS : [ \t\r\n]+ -> skip ;
