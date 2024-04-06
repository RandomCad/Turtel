grammar Scene;

// Parser rules
number: Num     
      | Float   #Float
      ; 
var   : ID      # Variable
      | IncID   # GlobalVariable
      ;

// Lexer rules
Num   : '-'? [0-9]+ ;
Float : '-'? [0-9]+ '.' [0-9]+
      | '-'? '.' [0-9]+ ;

ID    : [a-z]+ ; // [_a-zA-Z0-9] [_@a-zA-Z0-9]* ;
IncID : '@' [_@a-zA-Z0-9]* ;

t: .*;

WS : [ \t\r\n]+ -> skip ;
