grammar Scene;

file  : (pathdef | calcdef)* main (pathdef |calcdef)*; 

main    : 'begin' stat* 'end';
pathdef : 'a';
calcdef : 'a';

stat    : walk
        | save
        | walkBack
        | jump
        | jumpBack
        | waklHome
        | jumpHome
        | turnLeft
        | turnRight
        | direction
        | clear
        | stop
        | finish
        ;

walk  	  : Walk expr ;
save      : Save ID ; ///<Save the drawen immage to the named Bitmap
walkBack  : Walk Back expr; ///<walk in the opposit direktion of the looking dir
jump      : Jump expr ; ///< same as walk without drawing
jumpBack  : Jump Back expr ;
waklHome  : Walk Home; ///< draw a line to the home position
jumpHome  : Jump Home; ///< jump to the home position
turnLeft  : Turn Left expr; ///< turn x deg to the left (positiv)
turnRight : Turn Right expr; ///< turn x deg to the right (negative)
direction : Direction expr; ///< set the looking direktion to expr from the global 0(up)
clear     : Clear; ///< reset the screen to basecollore
stop      : Stop      ///< end the program and wait for user input
          | Stop expr;///< return code is expr (rounded)
finish    : Finish
          | Finish expr; ///< emediatly return


// Parser rules
expr  : ( klamKon | number) '^' (klamKon | number) #Exp
      | ( klamKon | number) '*' (klamKon | number) #Mult
      | ( klamKon | number) '/' (klamKon | number) #Dife
      | expr '+' expr #Add
      | expr '-' expr #Dim
      | '|' expr '|'  #ABS
      | '-' number    #Negate
      | number	      #NumExpr	
      | var	      #VarExpr
      ;
klamKon	: '(' expr ')' ;
number: Num     #Int
      | Float   #Float
      ; 
var   : ID      #Variable
      | IncID   #GlobalVariable
      ;

// Lexer rules
Walk	: 'walk';
Save  : 'save';
Back  : 'back';
Jump  : 'jump';
Home  : 'home';
Left  : 'left';
Turn  : 'turn';
Stop  : 'stop';
Right : 'right';
Clear : 'clear';
Finish: 'finish';
Direction : 'direction';

Num   : [0-9]+ ;
Float : [0-9]+ '.' [0-9]+
      | '.' [0-9]+ ;

ID    : [_a-zA-Z] [_@a-zA-Z0-9]* ;
IncID : '@' [_@a-zA-Z0-9]* ;

WS : [ \t\r\n]+ -> skip ;
