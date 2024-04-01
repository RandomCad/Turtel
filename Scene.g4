grammar Scene;

// Parser rules
number: Num     
      | Float   #Float
      ; 

// Lexer rules
Num   : '-'? [0-9]+ ;
Float : '-'? [0-9]+ '.' [0-9]+
      | '-'? '.' [0-9]+ ;
t : '.*';

WS : [ \t\r\n]+ -> skip ;
