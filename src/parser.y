%{
#include <stdio.h>
#include <lexer.h>
#include <symbol.h>

int yylex();
void exprprint(int);
%}

%union {
	struct ast_node *n;
}

%token TOKEOF IDENT CHARLIT STRING NUMBER INDSEL PLUSPLUS MINUSMINUS SHL SHR LTEQ GTEQ EQEQ NOTEQ LOGAND LOGOR ELLIPSIS TIMESEQ DIVEQ MODEQ PLUSEQ MINUSEQ SHLEQ SHREQ ANDEQ OREQ XOREQ AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE _BOOL _COMPLEX _IMAGINARY

%right UNSIGNED SIGNED SHORT LONG INT CHAR DOUBLE
%right ')' ELSE

%start translation_unit

%%
/* From H&S C Ref book */
translation_unit: top_level_decl
				| translation_unit top_level_decl
				;
top_level_decl: decl
			  | func_def
			  ;
/** Functions **/
func_def: func_def_spec compound_stmt
		;
func_def_spec: decl_specs declarator decl_list
			 | decl_specs declarator
			 | declarator decl_list
			 | declarator
			 ;
decl_list: decl
		 | decl_list decl
		 ;
/** Declaration **/
decl: decl_specs init_declarator_list ';'
	;
decl_specs: storage_class_spec decl_specs
		  | storage_class_spec
		  | type_spec decl_specs
		  | type_spec
		  | type_qual decl_specs
		  | type_qual
		  | func_spec decl_specs
		  | func_spec
		  ;
init_declarator_list: init_declarator { }
					| init_declarator_list ',' init_declarator { }
					;
init_declarator: declarator
			   | declarator '=' initializer { }
			   ;
storage_class_spec: AUTO
				  | EXTERN
				  | REGISTER
				  | STATIC
				  | TYPEDEF
				  ;
type_spec: int_type_spec 
		 | flpt_type_spec
		 | void_type_spec
		 | struct_type_spec
		 | union_type_spec
		 ;
type_qual: CONST
		 | VOLATILE
		 | RESTRICT
		 ;
func_spec: INLINE
		 ;

/*** Declarator **/
declarator: pointer_declarator
          | direct_declarator
          ;
pointer_declarator: pointer direct_declarator
                  ;
pointer: '*'
       | '*' type_qual_list
       | '*' pointer
       | '*' type_qual_list pointer
       ;
direct_declarator: simple_declarator
                 | '(' declarator ')'
                 | func_declarator
                 | array_declarator
                 ;
simple_declarator: IDENT
                 ;
type_qual_list: type_qual
              | type_qual_list type_qual
              ;
array_declarator: direct_declarator '[' ']'
                | direct_declarator '[' const_expr ']'
                | direct_declarator '[' array_qual_list assign_expr ']'
                | direct_declarator '[' array_qual_list ']'
                | direct_declarator '[' assign_expr ']'
                | direct_declarator '[' array_qual_list '*' ']'
                | direct_declarator '[' '*' ']'
                ;
const_expr: cond_expr
          ;
array_qual_list: array_qual
               | array_qual_list array_qual
               ;
array_qual: STATIC
          | RESTRICT
          | CONST
          | VOLATILE
          ;
/*** Function ***/
func_declarator: direct_declarator '(' param_type_list ')'
               | direct_declarator '(' ident_list ')'
               | direct_declarator '(' ')'
               ;
param_type_list: param_list
                  | param_list ',' ELLIPSIS
                  ;
param_list: param_decl
             | param_list ',' param_decl
             ;
param_decl: decl_specs declarator
             | decl_specs abstract_declarator
             | decl_specs
             ;
ident_list: IDENT
          | param_list ',' IDENT
          ;
/*** Initializer ***/
initializer: assign_expr
           | '{' initializer_list '}'
           | '{' initializer_list ',' '}'
           ;
initializer_list: initializer
                | initializer_list ',' initializer
                | designation initializer
                | initializer_list ',' designation initializer
                ;
designation: designator_list '='
           ;
designator_list: designator
               | designator_list designator
               ;
designator: '[' const_expr ']'
          | '.' IDENT
          ;
/** Types **/
int_type_spec: signed_type_spec
             | unsigned_type_spec
             | char_type_spec
             | bool_type_spec
             ;
signed_type_spec: SHORT
                | SHORT INT
                | SIGNED SHORT
                | SIGNED SHORT INT
                | INT
                | SIGNED INT
                | SIGNED
                | LONG
                | LONG INT
                | SIGNED LONG
                | SIGNED LONG INT
                | LONG LONG
                | LONG LONG INT
                | SIGNED LONG LONG
                | SIGNED LONG LONG INT
                ;
unsigned_type_spec: UNSIGNED SHORT INT
                  | UNSIGNED SHORT
                  | UNSIGNED INT
                  | UNSIGNED
                  | UNSIGNED LONG INT
                  | UNSIGNED LONG
                  | UNSIGNED LONG LONG INT
                  | UNSIGNED LONG LONG
                  ;
char_type_spec: CHAR
              | SIGNED CHAR
              | UNSIGNED CHAR
              ;
bool_type_spec: _BOOL
              ;
flpt_type_spec: FLOAT
              | DOUBLE
              | LONG DOUBLE
              | complx_type_spec
              ;
complx_type_spec: FLOAT _COMPLEX
                | DOUBLE _COMPLEX
                | LONG DOUBLE _COMPLEX
                ;
void_type_spec: VOID
              ;
struct_type_spec: struct_type_def
                | struct_type_ref
                ;
struct_type_def: STRUCT struct_tag '{' field_list '}'
               | STRUCT '{' field_list '}'
               ;
struct_type_ref: STRUCT struct_tag
               ;
struct_tag: IDENT
          ;
field_list: component_decl
          | field_list component_decl
          ;
component_decl: type_spec component_declarator_list ';'
              ;
component_declarator_list: component_declarator
                         | component_declarator_list ',' component_declarator
                         ;
component_declarator: simple_component
                    | bit_field
                    ;
simple_component: declarator
                ;
bit_field: declarator ':' width
         | ':' width
         ;
width: const_expr
     ;
union_type_spec: union_type_def
               | union_type_ref
               ;
union_type_def: UNION union_tag '{' field_list '}'
              | UNION '{' field_list '}'
              ;
union_type_ref: UNION union_tag
              ;
union_tag: IDENT
         ;
type_name: decl_specs abstract_declarator
         | decl_specs
         ;
abstract_declarator: pointer
                   | pointer direct_abstract_declarator
                   | direct_abstract_declarator
                   ;
direct_abstract_declarator: '(' abstract_declarator ')'
                        | direct_abstract_declarator '[' const_expr ']'
                        | '[' const_expr ']'
                        | direct_abstract_declarator '['expr ']'
                        | '[' expr ']'
                        | direct_abstract_declarator '[' '*' ']'
                        | '[' '*' ']'
                        | direct_abstract_declarator '(' param_type_list ')'
                        | direct_abstract_declarator '(' ')'
                        ;
/** Expressions **/
primary_expr: IDENT { }
           | NUMBER
           | paren_expr
           ;
paren_expr: '(' expr ')'
         ;
postfix_expr: primary_expr
           | subscript_expr
           | comp_sel_expr
           | func_call
           | postinc_expr
           | postdec_expr
           | compound_lit
           ;
subscript_expr: postfix_expr '[' expr ']'
             ;
comp_sel_expr: direct_comp_sel
             | indirect_comp_sel
             ;
direct_comp_sel: postfix_expr '.' IDENT
               ;
indirect_comp_sel: postfix_expr INDSEL IDENT
                 ;
func_call: postfix_expr '(' expr_list ')'
         | postfix_expr '(' ')'
         ;
expr_list: assign_expr
         | expr_list ',' assign_expr
         ;
postinc_expr: postfix_expr PLUSPLUS
            ;
postdec_expr: postfix_expr MINUSMINUS
            ;
compound_lit: '(' type_name ')' '{' initializer_list ',' '}'
            | '(' type_name ')' '{' initializer_list '}'
            ;
cast_expr: unary_expr
         | '(' type_name ')' cast_expr
         ;
unary_expr: postfix_expr
          | sizeof_expr
          | unary_minus_expr
          | unary_plus_expr
          | logical_neg_expr
          | bitwise_neg_expr
          | addr_expr
          | indirect_expr
          | preinc_expr
          | predec_expr
          ;
sizeof_expr: SIZEOF '(' type_name ')'
           | SIZEOF unary_expr
           ;
unary_minus_expr: '-' cast_expr
                ;
unary_plus_expr: '+' cast_expr
               ;
logical_neg_expr: '!' cast_expr
                ;
bitwise_neg_expr: '~' cast_expr
                ;
addr_expr: '&' cast_expr
         ;
indirect_expr: '*' cast_expr
             ;
preinc_expr: PLUSPLUS unary_expr
           ;
predec_expr: MINUSMINUS unary_expr { }
           ;
mult_expr: cast_expr
         | mult_expr mult_op cast_expr
         ;
mult_op: '*'
       | '/'
       | '%'
       ;
add_expr: mult_expr
        | add_expr add_op mult_expr
        ;
add_op: '+'
      | '-'
      ;
shift_expr: add_expr
          | shift_expr shift_op add_expr
          ;
shift_op: SHR
        | SHL
        ;
rel_expr: shift_expr
        | rel_expr rel_op shift_expr
        ;
rel_op: '<'
      | '>'
      | LTEQ
      | GTEQ
      ;
equal_expr: rel_expr
          | equal_expr equal_op rel_expr
          ;
equal_op: EQEQ
        | NOTEQ
        ;
bit_or_expr: bit_xor_expr
           | bit_or_expr '|' bit_xor_expr
           ;
bit_xor_expr: bit_and_expr
            | bit_xor_expr '^' bit_and_expr
            ;
bit_and_expr: equal_expr
            | bit_and_expr '&' equal_expr
            ;
log_or_expr: log_and_expr
           | log_or_expr LOGOR log_and_expr
           ;
log_and_expr: bit_or_expr
            | log_and_expr LOGAND bit_or_expr
            ;
cond_expr: log_or_expr
         | log_or_expr '?' expr ':' cond_expr
         ;
assign_expr: cond_expr
           | unary_expr assign_op assign_expr { }
           ;
assign_op: '='
         | PLUSEQ
         | MINUSEQ
         | TIMESEQ
         | DIVEQ
         | MODEQ
         | SHLEQ
         | SHREQ
         | ANDEQ
         | XOREQ
         | OREQ
         ;
comma_expr: assign_expr
          | comma_expr ',' assign_expr { }
          ;
expr: comma_expr
    ;
/** Statements **/
stmt: expr_stmt
    | label_stmt
    | compound_stmt
    | cond_stmt
    | iter_stmt
    | switch_stmt
    | break_stmt
    | cont_stmt
    | ret_stmt
    | goto_stmt
    | null_stmt
    ;
cond_stmt: if_stmt
         | if_else_stmt
         ;
iter_stmt: do_stmt
         | while_stmt
         | for_stmt
         ;
expr_stmt: expr ';' { }
         ;
label_stmt: label ':' stmt
          ;
label: named_label
     | case_label
     | default_label
     ;
compound_stmt: '{' { } decl_or_stmt_list '}' { }
             | '{' '}'
             ;
decl_or_stmt_list: decl_or_stmt
                 | decl_or_stmt_list decl_or_stmt
                 ;
decl_or_stmt: decl
            | stmt
            ;
if_stmt: IF '(' expr ')' stmt
       ;
if_else_stmt: IF '(' expr ')' stmt ELSE stmt
            ;
while_stmt: WHILE '(' expr ')' stmt
          ;
do_stmt: DO stmt WHILE '(' expr ')'
       ;
for_stmt: FOR for_expr stmt
        ;
for_expr: '(' init_clause ';' expr ';' expr ')'
        | '(' init_clause ';' expr ';' ')'
        | '(' init_clause ';' ';' expr ')'
        | '(' init_clause ';' ';' ')'
        | '(' ';' expr ';' expr ')'
        | '(' ';' expr ';' ')'
        | '(' ';' ';' expr ')'
        | '(' ';' ';'  ')'
        ;
init_clause: expr
           | decl
           ;
switch_stmt: SWITCH '(' expr ')' stmt
           ;
case_label: CASE const_expr
          ;
default_label: DEFAULT
             ;
break_stmt: BREAK ';'
          ;
cont_stmt: CONTINUE ';'
         ;
ret_stmt: RETURN expr ';'
        | RETURN ';'
        ;
goto_stmt: GOTO named_label ';'
         ;
named_label: IDENT
           ;
null_stmt: ';'
         ;
%%

void exprprint(int val){
	printf("%s:%d: exprval=%d\n", filename, yylineno, val);
}

int main(){
	global = current = new_sym_table();
	return yyparse();
}

