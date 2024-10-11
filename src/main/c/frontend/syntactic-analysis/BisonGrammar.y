%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals */
	char * single_type;
	char * id;
	char * value;
	Token token;

	/** Non-terminals */
	Declaration * declaration; 
	Type * type;
	Interface * interface;
	VariableType * variableType;
	AssignmentList * assignmentList;
	VariableList * variableList;
	Enum * enum;

	Program * program;

	AssignOperator * assignOperator;
	Assign * assign;

	Expression * expression;
	AritmeticOperator * arithmeticOperator;
	LogicalOperator * logicalOperator;
	ComparisonOperator * comparisonOperator;

	ReturnType * returnType;
	PromiseReturnType * promiseReturnType;

	Code * code;
	ValueList * valueList;
	AsyncFunction * asyncFunction;
	VariableModifier * variableModifier;
	IterableVariable * iterableVariable;
	ParamsFor * paramsFor;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <token> FUNCTION
%token <token> IF
%token <token> FOR
%token <token> INTERFACE
%token <token> ENUM
%token <token> ELSE
%token <token> OF
%token <token> IN

%token <token> LET
%token <token> CONST
%token <token> VAR

%token <single_type> SINGLE_TYPE

%token <id> ID 
%token <token> COLON
%token <token> PIPE
%token <token> EQUAL
%token <value> VALUE

%token <token> SEMI_COLON 
%token <token> COMA
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS


%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

%token <token> OR
%token <token> AND
%token <token> GREATER
%token <token> LESS
%token <token> NEQUAL
%token <token> STRICT_EQUAL
%token <token> STRICT_NEQUAL
%token <token> LESS_EQUAL
%token <token> GREATER_EQUAL

%token <token> ADD_ASSIGN
%token <token> SUB_ASSIGN
%token <token> MUL_ASSIGN
%token <token> DIV_ASSIGN

%token <token> ARROW
%token <token> AYSNC
%token <token> PROMISE


/** Non-terminals. */
%type <type> type
%type <assign> assign
%type <assignOperator> assignOperator
%type <variableType> variableType
%type <declaration> declaration
%type <interface> interface
%type <assignmentList> assignmentList
%type <variableList> variableList
%type <returnType> returnType
%type <enum> enum
%type <arithmeticOperator> arithmeticOperator
%type <logicalOperator> logicalOperator
%type <expression> expression
%type <comparisonOperator> comparisonOperator
%type <program> program
%type <code> code
%type <valueList> valueList
%type <asyncFunction> asyncFunction
%type <promiseReturnType> promiseReturnType
%type <variableModifier> variableModifier
%type <iterableVariable> iterableVariable
%type <paramsFor> paramsFor


/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: code
	;

code: if code
	| for code
	| declaration code
	| enum code
	| interface code
	| functionCall code
	| function code
	| arrowFunction code
	| asyncFunction code
	| %empty
	;

// Expression -----------------------------------------------------------------------------------------------------------------
arithmeticOperator: ADD         													{ $$ = ADD_OP; }
	| SUB      																		{ $$ = SUB_OP; }
	| MUL 																			{ $$ = MUL_OP; }
	| DIV 																			{ $$ = DIV_OP; }
	;

comparisonOperator: GREATER       													{ $$ = GREATER_OP; }
    | LESS         																	{ $$ = LESS_OP; }
    | NEQUAL       																	{ $$ = NEQUAL_OP; }
    | STRICT_EQUAL																	{ $$ = STRICT_EQUAL_OP; }
    | STRICT_NEQUAL 																{ $$ = STRICT_NEQUAL_OP; }
    | LESS_EQUAL   																	{ $$ = LESS_EQUAL_OP; }
    | GREATER_EQUAL 																{ $$ = GREATER_EQUAL_OP; }
    ;

logicalOperator: OR 																{ $$ = OR_OP; }
	| AND 																			{ $$ = AND_OP; }
	;

expression: expression[left] arithmeticOperator expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, arithmeticOperator); }
	| expression[left] logicalOperator expression[right]							{ $$ = LogicalExpressionSemanticAction($left, $right, logicalOperator); }
	| expression[left] comparisonOperator expression[right]							{ $$ = ComparisonExpressionSemanticAction($left, $right, comparisonOperator); }
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS									{ $$ = FactorSemanticAction($2); }
	| ID 
	| VALUE
	| functionCall
	;

// Type -------------------------------------------------------------------------------------------------------------------------------------------------
type: SINGLE_TYPE																    { $$ = SingleTypeSemanticAction($1); }
	| SINGLE_TYPE PIPE type															{ $$ = UnionTypeSemanticAction($1, $3); }
	;

variableType: ID COLON type															{ $$ = VariableSemanticAction($1, $3); }
	| ID
	;

returnType: COLON type																{ $$ = ReturnTypeSemanticAction($2); } // debe haber mas casos 
	;

promiseReturnType: PROMISE GREATER returnType GREATER								{ $$ = PromiseReturnTypeSemanticAction($2); }
	;

// Variable declaration and assignment -----------------------------------------------------------------------------------------------------------------
declaration: LET variableType assign											    { $$ = LetDeclarationSemanticAction($2, $4); }
	| CONST variableType EQUAL VALUE 							       				{ $$ = ConstDeclarationSemanticAction($2,$4); }
	| VAR variableType assign														{ $$ = VarDeclarationSemanticAction($2, $4); }
	| declaration COMA variableType													{ $$ = MultipleDeclarationSemanticAction($1, $2); } // pensar mejor  VER
	;

assignOperator: EQUAL 															    { $$ = EQUAL_OP; }
	| ADD_ASSIGN 																	{ $$ = ADD_ASSIGN_OP; }
	| SUB_ASSIGN 																	{ $$ = SUB_ASSIGN_OP; } 
	| MUL_ASSIGN 																	{ $$ = MUL_ASSIGN_OP; }
	| DIV_ASSIGN 																	{ $$ = DIV_ASSIGN_OP; }
	;

assign: assignOperator expression													{ $$ = AssignOperatorAssignSemanticAction($1, $2); } // creo que solo aplica a number VER 
	;

assignmentList: variableType assign													{ $$ = AssignListSemanticAction($1); }
	| variableType assign COMA assignmentList  										{ $$ = MultipleAssignListSemanticAction($1, $3); }
	| variableType COMA assignmentList												{ $$ = MultipleAssignListSemanticAction($1, $3); }
	;

variableList: variableType															{ $$ = InterfaceListSemanticAction($1); }
	| variableType COMA variableList												{ $$ = MultipleInterfaceListSemanticAction($1, $2); }
	;

valueList: VALUE																	{ $$ = ValueListSemanticAction($1); }
	| valueList COMA VALUE															{ $$ = MultipleValueListSemanticAction($1, $3); }
	;

variableModifier: LET
	| CONST																			{ $$ = VariableModifierSemanticAction($1); }
	| VAR																			{ $$ = VariableModifierSemanticAction($1); }
	;

iterableVariable: ID  																{ $$ = IterableVariableSemanticAction($1); }
	| functionCall 																	{ $$ = IterableVariableSemanticAction($1); }
	;

// Enum and Interface -----------------------------------------------------------------------------------------------------------------
enum: ENUM ID OPEN_BRACE assignmentList CLOSE_BRACE									{ $$ = EnumSemanticAction($2, $4); }
	;

interface: INTERFACE ID OPEN_BRACE declaration CLOSE_BRACE							{ $$ = InterfaceSemanticAction($2, $4); }
	;

// if -----------------------------------------------------------------------------------------------------------------
if: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE ELSE OPEN_BRACE code CLOSE_BRACE
	;
//for ... of, no con for ... in -----------------------------------------------------------------------------------------------------------------
paramsFor: OPEN_PARENTHESIS declaration SEMI_COLON expression SEMI_COLON assign CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS variableModifier ID OF iterableVariable CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS variableModifier ID IN iterableVariable CLOSE_PARENTHESIS
	;

for: FOR paramsFor OPEN_BRACE code CLOSE_BRACE
	;

// function -----------------------------------------------------------------------------------------------------------------
functionCall: ID OPEN_PARENTHESIS valueList CLOSE_PARENTHESIS
	;

function: FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS returnType OPEN_BRACE code CLOSE_BRACE
	| FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE
	;

arrowFunction: OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS ARROW COLON returnType OPEN_BRACE code CLOSE_BRACE
	| OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS ARROW OPEN_BRACE code CLOSE_BRACE
	;

asyncFunction: AYSNC FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS COLON promiseReturnType OPEN_BRACE code CLOSE_BRACE
 	| AYSNC FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE
	;

%%
