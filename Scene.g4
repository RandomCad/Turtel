grammar Scene;

// Lexer rules
Walk	: 'walk'          ;
Save  : 'save'          ;
Back  : 'back'          ;
Jump  : 'jump'          ;
Home  : 'home'          ;
Left  : 'left'          ;
Turn  : 'turn'          ;
Stop  : 'stop'          ;
Right : 'right'         ;
Clear : 'clear'         ;
Finish: 'finish'        ;
Direction : 'direction' ;
Mark  : 'mark'          ;
Store : 'store'         ;
In    : 'in'            ;
Step  : 'step'          ;
While : 'while'         ;
Untile: 'untile'        ;
Begin : 'begin'         ;
End   : 'end'           ;
Color : 'color'         ;

Num   : [0-9]+ ;
Float : [0-9]+ '.' [0-9]+
      | '.' [0-9]+ ;

ID    : [_a-zA-Z] [_@a-zA-Z0-9]* ;
IncID : '@' [_@a-zA-Z0-9]* ;

file  : (pathdef | calcdef)* main (pathdef |calcdef)*; 

main    : Begin statList End;
pathdef : 'path' ID paramlist? statList 'endpath' ;
calcdef : 'calculation' ID  paramlist statList 'returns' expr 'endcalc' ;
paramlist : '(' (var ',')* var ')'
          | '(' ')'
          ;
statList  : stat* ;

stat    : walk        | save        | jump
        | walkHome    | jumpHome    | turnLeft
        | turnRight   | direction   | clear
        | stop        | finish      | storeVar
        | addVar      | subVar      | divVar
        | multVar     | walkMark    | jumpMark
        | mark        | colorCmd    | if
        | toFor       | pathCall
        | for         | while       | doUntil
        ;

///conditions/loops
if      : 'if' cond 'then' stat+ else? 'endif' ;
else    : 'else' stat+ ; //helper for if;
toFor   : 'do' expr 'times' stat+ 'done';
for     : 'counter' var 'from' expr 'to' expr 'do' stat+ 'done' #simpUpFor
        | 'counter' var 'from' expr 'to' expr Step expr 'do' stat+ 'done' #stepUpFor
        | 'counter' var 'from' expr 'downto' expr 'do' stat+ 'done' #simpDownFor
        | 'counter' var 'from' expr 'downto' expr Step expr 'do' stat+ 'done' #stepDownFor
        ; //could all be implemented with stepUpFor
while   : While cond 'do' stat+ 'done';
doUntil : 'repeat' stat+ Untile cond;

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
walkHome  : Walk Home; ///< draw a line to the home position
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
pathCall  : 'path' ID paramlist? ;

///conditions (if)
cond  : expr '<' expr #lesThan
      | expr '>' expr #greaterThan
      | expr '<=' expr #lesEqThan
      | expr '>=' expr #greaterEqThan
      | expr '=' expr #Equal
      | expr '<>' expr #Unequal
      | '(' cond ')' #clamCond
      | 'NOT' cond #notCond
      | cond 'AND' cond #andCond
      | cond 'OR' cond #orCond
      ;

///Math expressions
expr  : expr '^' expr #Exp
      | expr '*' expr #Mult
      | expr '/' expr #Dife
      | expr '-' expr #Dim
      | expr '+' expr #Add
      | '|' expr '|'  #ABS
      | '-' ( number | klamKon | var)   #Negate
      | klamKon #ClamExpr
      | number	      #NumExpr	
      | var	      #VarExpr
      | ID paramlist  #funcCall
      ;
klamKon	: '(' expr ')' ;
number: Num     #Int
      | Float   #Float
      ; 
var   : ID      #Variable
      | IncID   #GlobalVariable
      | '@pi'     #piVar
      ;

WS : [ \t\r\n]+ -> skip ;
COMMENT : '"' ~[\r\n]* -> skip ;
