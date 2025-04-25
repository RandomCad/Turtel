grammar Commandline;

args      : FileName option FileName option turtelArg? ;

option    : ( output | interp | allowInf | disalInf )* ;
output    : '--output' FileName ;
interp    : '--interpret' ;
allowInf  : '--allow-infinitloop' ;
disalInf  : '--disallow-infinitloop' ;
turtelArg : '--' Float* ;

FileName  : ('/' | './' | '../')? [a-zA-Z0-9._-]+ ('/' [a-zA-Z0-9._-]+)* ; 

Float : [0-9]+ '.' [0-9]+
      | '.' [0-9]+ ;

WS : [ \t\r\n\f]+ -> skip ;
