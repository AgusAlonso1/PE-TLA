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
	char *error;
	Token token;

	/** Non-terminals */
	Declaration * declaration; 

	Type * type;
	VariableType * variableType;
	AssignmentList * assignmentList;
	VariableList * variableList;
	ValueList * valueList;
	Enum * enum;
	Interface * interface;

	AssignOperator * assignOperator;
	Assign * assign;

	Expression * expression;
	AritmeticOperator * arithmeticOperator;
	LogicalOperator * logicalOperator;
	ComparisonOperator * comparisonOperator;
	UnaryOperator * unaryOperator;
	UnaryExpresion * unaryExpresion;
	Factor * factor;

	VariableModifier * variableModifier;
	IterableVariable * iterableVariable;
	ParamsFor * paramsFor;
	For * for;
	If *if;
	While * while;
	TryCatch * tryCatch;

	ReturnType * returnType;
	PromiseReturnType * promiseReturnType;

	Function * function; 
	ArrowFunction * arrowFunction;
	AsyncFunction * asyncFunction;
	Await * await;
	FunctionCall * functionCall;

	Code * code;
	Program * program;
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
%token <token> WHILE
%token <token> INTERFACE
%token <token> ENUM
%token <token> ELSE
%token <token> OF
%token <token> IN

%token <token> LET
%token <token> CONST
%token <token> VAR

%token <single_type> SINGLE_TYPE
%token <error> ERROR 
%token <token> CONSOLE_ERROR

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

%token <token> INCREMENT
%token <token> DECREMENT

%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

%token <token> OR
%token <token> AND
%token <token> NOT
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
%token <token> AWAIT
%token <token> TRY 
%token <token> CATCH 


/** Non-terminals. */
%type <declaration> declaration
%type <type> type

%type <variableType> variableType
%type <variableModifier> variableModifier
%type <iterableVariable> iterableVariable

%type <valueList> valueList
%type <assignmentList> assignmentList
%type <variableList> variableList


%type <assign> assign
%type <assignOperator> assignOperator

%type <enum> enum
%type <interface> interface

%type <arithmeticOperator> arithmeticOperator
%type <logicalOperator> logicalOperator
%type <comparisonOperator> comparisonOperator
%type <unaryOperator> unaryOperator
%type <unaryExpresion> unaryExpresion
%type <expression> expression


%type <returnType> returnType
%type <promiseReturnType> promiseReturnType
%type <await> await

%type <if> if
%type <for> for
%type <while> while
%type <tryCatch> tryCatch

%type <function> function
%type <arrowFunction> arrowFunction
%type <asyncFunction> asyncFunction
%type <functionCall> functionCall

%type <paramsFor> paramsFor


%type <factor> factor
%type <program> program
%type <code> code



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

unaryOperator: INCREMENT 															{ $$ = INC_OP; }
	| DECREMENT 																	{ $$ = DEC_OP; }
	;

unaryExpresion: ID unaryOperator 													{ $$ = 	UnaryExpressionSemanticAction($1,unaryOperator, right); }
	| OPEN_PARENTHESIS unaryExpresion CLOSE_PARENTHESIS unaryOperator				{ $$ = 	UnaryExpressionSemanticAction($1,unaryOperator, right); }
	| unaryOperator ID 																{ $$ =  UnaryExpressionSemanticAction($1,unaryOperator, left); }
	| unaryOperator OPEN_PARENTHESIS unaryExpresion CLOSE_PARENTHESIS				{ $$ =  UnaryExpressionSemanticAction($1,unaryOperator, left); }
	;

expression: expression[left] arithmeticOperator expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, arithmeticOperator); }
	| expression[left] logicalOperator expression[right]							{ $$ = LogicalExpressionSemanticAction($left, $right, logicalOperator); }
	| expression[left] comparisonOperator expression[right]							{ $$ = ComparisonExpressionSemanticAction($left, $right, comparisonOperator); }
	| NOT expression 																{ $$ = NotExpressionSemanticAction($1); }
	| factor																		{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS								{ $$ = ExpressionFactorSemanticAction($2); }
	| ID 																			{ $$ = VariableFactorSemanticAction($1); }
	| VALUE 																		{ $$ = ValueFactorSemanticAction($1); }
	| functionCall 																	{ $$ = FunctionFactorSemanticAction($1) }
	;
	
// Type -------------------------------------------------------------------------------------------------------------------------------------------------
type: SINGLE_TYPE																    { $$ = SingleTypeSemanticAction($1); }
	| SINGLE_TYPE PIPE type															{ $$ = UnionTypeSemanticAction($1, $3); }
	;

variableType: ID COLON type															{ $$ = VariableTypeSemanticAction($1, $3); }
	| ID																			{ $$ = VariableTypeSemanticAction($1, NULL); } // CHECK
	;

returnType: COLON type																{ $$ = ReturnTypeSemanticAction($2); }
	| %empty 																		{ $$ = ReturnTypeSemanticAction(NULL); } //check
	;

promiseReturnType: PROMISE GREATER returnType LESS									{ $$ = PromiseReturnTypeSemanticAction($2); }
	| %empty																		{ $$ = PromiseReturnTypeSemanticAction(NULL); }
	;

// Variable declaration and assignment -----------------------------------------------------------------------------------------------------------------
declaration: LET variableType EQUAL expression										{ $$ = DeclarationSemanticAction(0, $2, $4); }
	| LET variableType																{ $$ = DeclarationSemanticAction(0, $2, NULL); }
	| CONST variableType EQUAL expression 								       		{ $$ = DeclarationSemanticAction(1, $2, $4); }
	| VAR variableType EQUAL expression												{ $$ = DeclarationSemanticAction(2, $2, $4); }
	| VAR variableType																{ $$ = DeclarationSemanticAction(2, $2, NULL); }
//	| declaration COMA variableType													{ } // VER
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

valueList: expression																	{ $$ = ValueListSemanticAction($1); }
	| expression COMA valueList															{ $$ = MultipleValueListSemanticAction($1, $3); }
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

interface: INTERFACE ID OPEN_BRACE variableList CLOSE_BRACE							{ $$ = InterfaceSemanticAction($2, $4); }
	;

// if -----------------------------------------------------------------------------------------------------------------
if: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 										{ $$ = IfSemanticAction($3, $6, NULL); }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE ELSE OPEN_BRACE code CLOSE_BRACE 	{ $$ = IfSemanticAction($3, $6, $10); }
	;
	
//for -----------------------------------------------------------------------------------------------------------------
paramsFor: OPEN_PARENTHESIS declaration SEMI_COLON expression SEMI_COLON assign CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS variableModifier ID OF iterableVariable CLOSE_PARENTHESIS
	| OPEN_PARENTHESIS variableModifier ID IN iterableVariable CLOSE_PARENTHESIS
	;

for: FOR paramsFor OPEN_BRACE code CLOSE_BRACE
	;

//while -----------------------------------------------------------------------------------------------------------------
while: WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 									{ $$ = WhileSemanticAction($3, $6); }
	;

// try catch -----
await: AWAIT expression
	;

tryCatch: TRY OPEN_BRACE code CLOSE_BRACE CATCH OPEN_PARENTHESIS ERROR CLOSE_PARENTHESIS OPEN_BRACE CONSOLE_ERROR CLOSE_BRACE
	;


// function -----------------------------------------------------------------------------------------------------------------
functionCall: ID OPEN_PARENTHESIS valueList CLOSE_PARENTHESIS 															{ $$ = functionCallSemanticAction($1,$3); }
	;

function: FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS returnType OPEN_BRACE code CLOSE_BRACE { $$ = FunctionSemanticAction($2,$4,$6,$8)}
	;

arrowFunction: OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS ARROW COLON returnType OPEN_BRACE code CLOSE_BRACE { $$ = ArrowFunctionSemanticAction($2,$5,$8); }
	;

asyncFunction: AYSNC FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS COLON promiseReturnType OPEN_BRACE code CLOSE_BRACE { $$ = AsyncFunctionSemanticAction($3,$4,$8,$10); }
	;

%%
