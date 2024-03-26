grammar Scene;

// Parser rules
t : '.*';

NEWLINE     : ('\r'? '\n' | '\r')+ ;
TAB         : ('\t' | '        ' | '    ' ) ;
WHITESPACE  : ' ' -> skip ;
