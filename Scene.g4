grammar Scene;

file  : (pathdef | calcdef)* main (pathdef |calcdef)*; 

main    : 'begin' stat* 'end';
pathdef : 'a';
calcdef : 'a';

stat    : walk        | save        | jump
        | waklHome    | jumpHome    | turnLeft
        | turnRight   | direction   | clear
        | stop        | finish      | storeVar
        | addVar      | subVar      | divVar
        | multVar     | walkMark    | jumpMark
        | mark        | colorCmd
        ;
///Variable commands
storeVar: 'store' expr 'in' var ;
addVar  : 'add' expr 'to' var;
subVar  : 'sub' expr 'from' var;
divVar  : 'div' var 'by' expr;
multVar : 'mul' var 'by' expr;

///move commands
walk  	  : Walk expr       #WalkFront
          | Walk Back expr  #WalkBack
          ;
jump      : Jump expr       #JumpFront
          | Jump Back expr  #JumpBack
          ; ///< same as walk without drawing
save      : Save ID ; ///<Save the drawen immage to the named Bitmap
waklHome  : Walk Home; ///< draw a line to the home position
jumpHome  : Jump Home; ///< jump to the home position
turnLeft  : Turn Left expr; ///< turn x deg to the left 
turnRight : Turn (Right)? expr; ///< turn x deg to the right 
direction : Direction expr; ///< set the looking direktion to expr from the global 0(up)
clear     : Clear; ///< reset the screen to basecollore
stop      : Stop #StopOK     ///< end the program and wait for user input
          | Stop expr #StopError ///< return code is expr (rounded)
          ;
finish    : Finish #FinOK
          | Finish expr #FinError ///< emediatly return
          ;
mark      : Mark;
walkMark  : Walk Mark;
jumpMark  : Jump Mark;
colorCmd  : Color expr expr expr;

// Parser rules
expr  : ( klamKon | number) '^' (klamKon | number) #Exp
      | ( klamKon | number) '*' (klamKon | number) #Mult
      | ( klamKon | number) '/' (klamKon | number) #Dife
      | expr '+' expr #Add
      | expr '-' expr #Dim
      | '|' expr '|'  #ABS
      | '-' ( number | klamKon )   #Negate
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
Mark: 'mark';
Color: 'color';

Store : 'store';
In    : 'in';

Num   : [0-9]+ ;
Float : [0-9]+ '.' [0-9]+
      | '.' [0-9]+ ;

ID    : [_a-zA-Z] [_@a-zA-Z0-9]* ;
IncID : '@' [_@a-zA-Z0-9]* ;

WS : [ \t\r\n]+ -> skip ;
