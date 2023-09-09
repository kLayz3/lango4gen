// First match gets accepted always! 
// So lexer rules should be in priority order!

grammar Go4Unpack;

///////////////////////
// // PARSER rules // //
///////////////////////

start : 
	(include_header)* ( class_structure )+ 
	;
	
class_structure : 
	class_declaration '{' class_body '};' 
	;

class_declaration : 
	name '(' (class_templates)? ')' 
	;

class_templates : 
	IDENT (';' IDENT)* 
	;

class_body : 
	(basic_field)* (basic_match)* (for_loop)*
	;

for_loop :
	FOR NORMAL_LEFT for_left_bound '<=' for_id '<' for_right_bound NORMAL_RIGHT 
	'{' loop_body '}'
	;

loop_body :
	(basic_field)* (basic_match)*
	;

include_header : 
	'#' INCLUDE STRING 
	;



basic_match : 
	name ':' type EQUALS MATCH NORMAL_LEFT number NORMAL_RIGHT SEMICOLON 
	;

basic_field : 
	name ':' type SEMICOLON
	;
// ------------------------- //

for_left_bound :
	DEC_NUMBER 
	;
for_id :
	IDENT 
	;
for_right_bound :
	DEC_NUMBER
	;

number : 
	DEC_NUMBER | HEX_NUMBER | BIN_NUMBER 
	;

name: 
	IDENT 
	;

type: 
	FIELD_TYPE
	;

//annotation IDNET 
//	'@' IDENT '~' 
//	;


///////////////////////
// // LEXER rules // //
///////////////////////

fragment W_CHAR    : [a-zA-Z0-9_-] ;
fragment W1_CHAR   : [a-zA-Z0-9_] ;
fragment LOWERCASE : [a-z] ;
fragment UPPERCASE : [A-Z] ;
fragment DIGIT     : [0-9] ;
fragment HEX_DIGIT : [0-9abcdef] ;
fragment BIN_DIGIT : [01] ;
fragment U32 : [iIuU]'32'('_t')? ;
fragment U16 : [iIuU]'16'('_t')? ;
fragment U8 : [iIuU]'8'('_t')? ;

CURLY_LEFT	 : '{' ;
CURLY_RIGHT  : '}' ;
SQUARE_LEFT	 : '[' ;
SQUARE_RIGHT : ']' ;
NORMAL_LEFT  : '(' ;
NORMAL_RIGHT : ')' ;

FOR       : 'for' ;
HASH      : '#' ;
INCLUDE   : 'include' ;
SEMICOLON : ';' ;
COMMA	  : ',' ;
EQUALS    : '=' ;
MATCH     : 'MATCH' ;

FIELD_TYPE : (U32 | U16 | U8) ;

OP		   : ( '+' | '-' | '*' | '%' | '<<' | '>>' );

IDENT      : ( LOWERCASE | UPPERCASE | '_') (W1_CHAR)* ;
FILE_NAME  : W_CHAR+ '.' W_CHAR+ ;

DEC_NUMBER : DIGIT+ ;
HEX_NUMBER : '0x' HEX_DIGIT+ ;
BIN_NUMBER : '0b' BIN_DIGIT+ ;
STRING     : '"' .*?  '"' ;

Whitespace   : [ \t]+    -> skip ;
Newline      : ('\r' '\n'? | '\n') -> skip; 
Comment		 : '/*' .*? '*/' -> skip ;
LineComment  : '//' ~[\r\n]* -> skip ;




