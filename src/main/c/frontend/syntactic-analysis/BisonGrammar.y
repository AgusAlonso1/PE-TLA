%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals */
	char * single_type;
	char * id;
	int value_int;
    float value_float;
    char *value_str;
    char value_bool;
	char * error;
	Token token;

	/** Non-terminals */
	Declaration * declaration; 
	TypeDeclaration * typeDeclaration;
	Type * type;
	VariableType * variableType;
	Constant * constant;
	ArrayContent * arrayContent;
	ObjectContent * objectContent;
	VariableList * variableList;
	VariableTypeList * variableTypeList;
	ArgumentList * argumentList;
	AssignOperator * assignOperator;
	Variable * variable;
	Expression * expression;
	IncDec * incDec;
	Factor * factor;
	IterableVariable * iterableVariable;
	ParamsFor * paramsFor;
	ForLoop * forLoop;
	IfStatement *ifStatement;
	WhileLoop * whileLoop;
	PromiseReturnType * promiseReturnType;
	FunctionDeclaration * functionDeclaration; 
	ArrowFunction * arrowFunction;
	AsyncFunction * asyncFunction;
	//Await * await;
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
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseConstant($$); } <constant>

%destructor { releaseVariable($$); } <variable>
%destructor { releaseVariableType($$); } <variableType>
%destructor { releaseVariableName($$); } <variableName>
%destructor { releaseVariableTypeList($$); } <variableList>
%destructor { releaseVariableValueList($$); } <variableValue>

%destructor { releaseDeclaration($$); } <declaration>

%destructor { releaseIf($$); } <ifStatement>
%destructor { releaseForParams($$); } <paramsFor>
%destructor { releaseFor($$); } <forLoop>
%destructor { releaseWhile($$); } <whileLoop>

%destructor { releaseFunctionCall($$); } <functionCall>
%destructor { releaseFunctionDeclaration($$); } <functionDeclaration>
%destructor { releaseArrowFunction($$); } <arrowFunction>
%destructor { releaseAsyncFunction($$); } <asyncFunction>

%destructor { releaseCode($$); } <code>
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
%token <token> RETURN

%token <token> LET
%token <token> CONST
%token <token> VAR

%token <single_type> SINGLE_TYPE
%token <token> TYPE

%token <id> ID 
%token <token> COLON
%token <token> PIPE
%token <token> EQUAL
%token <value_int> INT_VALUE
%token <value_float> FLOAT_VALUE
%token <value_str> STRING_VALUE
%token <value_bool> BOOL_VALUE

%token <token> SEMI_COLON 
%token <token> COMA
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET

%token <token> INCREMENT
%token <token> DECREMENT

%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

%token <token> ASSIGN
%token <token> GREATER
%token <token> LESS
%token <token> NEQUAL
%token <token> STRICT_EQUAL
%token <token> STRICT_NEQUAL
%token <token> LESS_EQUAL
%token <token> GREATER_EQUAL

%token <token> OR
%token <token> AND
%token <token> NOT

%token <token> ARROW
%token <token> ASYNC
%token <token> PROMISE
%token <token> AWAIT


/** Non-terminals. */
%type <declaration> declaration
%type <typeDeclaration> typeDeclaration
%type <type> type

%type <variableType> variableType
%type <constant> constant
%type <arrayContent> arrayContent
%type <objectContent> objectContent

%type <argumentList> argumentList
%type <variableList> variableList
%type <variableTypeList> variableTypeList


%type <variable> variable
%type <incDec> incDec
%type <expression> expression

%type <promiseReturnType> promiseReturnType
//%type <await> await

%type <ifStatement> ifStatement
/* %type <iterableVariable> iterableVariable
%type <paramsFor> paramsFor
%type <forLoop> forLoop */
%type <whileLoop> whileLoop

%type <functionDeclaration> functionDeclaration
%type <arrowFunction> arrowFunction
%type <asyncFunction> asyncFunction
%type <functionCall> functionCall

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

program: code 																																{ $$ = CodeProgramSemanticAction(currentCompilerState(),$1); }
	;

// Code -----------------------------------------------------------------------------------------------------------------
code: ifStatement code 																		 												{ $$ = IfCodeSemanticAction($1, $2); }
	//| forLoop code 																		 													{ $$ = ForCodeSemanticAction($1, $2); }
	| whileLoop code 																	 													{ $$ = WhileCodeSemanticAction($1, $2); }
	| declaration code															 	 														{ $$ = DeclarationCodeSemanticAction($1, $2); }
	| typeDeclaration code 																     												{ $$ = TypeDeclarationCodeSemanticAction($1, $2); }
	| functionCall code 															 														{ $$ = FunctionCallCodeSemanticAction($1, $2); }
	| functionDeclaration code 																 												{ $$ = FunctionDeclarationCodeSemanticAction($1, $2); }
	| arrowFunction code 															 														{ $$ = ArrowFunctionCodeSemanticAction($1, $2); }
	| asyncFunction code 															 														{ $$ = AsyncFunctionCodeSemanticAction($1, $2); }
	| expression code 															 															{ $$ = ExpressionCodeSemanticAction($1, $2); }
	| incDec code 																															{ $$ = IncDecCodeSemanticAction($1, $2); }
	| RETURN code 																															{ $$ = ReturnCodeSemanticAction($2); }
	| %empty																																{ $$ = NULL; }
	;

// Expression -----------------------------------------------------------------------------------------------------------------
incDec: expression INCREMENT 																											{ $$ = 	IncDecSemanticAction($1,INC_OP, POSTFIX); }
	| expression DECREMENT 																												{ $$ = 	IncDecSemanticAction($1,DEC_OP, POSTFIX); }
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS INCREMENT 																				{ $$ = 	IncDecSemanticAction($2,INC_OP, POSTFIX); }
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS DECREMENT 																				{ $$ = 	IncDecSemanticAction($2,DEC_OP, POSTFIX); }
	| INCREMENT expression 																												{ $$ =  IncDecSemanticAction($2,INC_OP, PREFIX); }
	| DECREMENT expression 																												{ $$ =  IncDecSemanticAction($2,DEC_OP, PREFIX); }
	| INCREMENT OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																				{ $$ =  IncDecSemanticAction($3,INC_OP, PREFIX); }
	| DECREMENT OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																				{ $$ =  IncDecSemanticAction($3,DEC_OP, PREFIX); }
	;

expression: expression[left] ADD expression[right]																						{ $$ = ExpressionSemanticAction($left, $right, ADD_OP); } 
	| expression[left] SUB expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, SUB_OP); }
	| expression[left] MUL expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, MUL_OP); }
	| expression[left] DIV expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, DIV_OP); }
	| expression[left] GREATER expression[right] 																						{ $$ = ExpressionSemanticAction($left, $right, GREATER_OP); }
	| expression[left] LESS expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, LESS_OP); }
	| expression[left] EQUAL expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, EQUAL_OP); }
	| expression[left] NEQUAL expression[right] 																						{ $$ = ExpressionSemanticAction($left, $right, NEQUAL_OP); }
	| expression[left] STRICT_EQUAL expression[right] 																					{ $$ = ExpressionSemanticAction($left, $right, STRICT_EQUAL_OP); }
	| expression[left] STRICT_NEQUAL expression[right] 																					{ $$ = ExpressionSemanticAction($left, $right, STRICT_NEQUAL_OP); }
	| expression[left] LESS_EQUAL expression[right] 																					{ $$ = ExpressionSemanticAction($left, $right, LESS_EQUAL_OP); }
	| expression[left] GREATER_EQUAL expression[right] 																					{ $$ = ExpressionSemanticAction($left, $right, GREATER_EQUAL_OP); }
	| expression[left] OR expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, OR_OP); }
	| expression[left] AND expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, AND_OP); }
	| NOT expression 																													{ $$ = ExpressionSemanticAction($2, NULL, NOT_OP);}
	| factor																															{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																					{ $$ = ExpressionFactorSemanticAction($2); }
	| ID																													    		{ $$ = VariableFactorSemanticAction($1); }
	| constant 																															{ $$ = ConstantFactorSemanticAction($1); }
	| functionCall 																														{ $$ = FunctionCallFactorSemanticAction($1); }
	;

// Type -------------------------------------------------------------------------------------------------------------------------------------------------
type: SINGLE_TYPE																    														{ $$ = SingleTypeSemanticAction($1, NULL); }
	| SINGLE_TYPE PIPE type																													{ $$ = SingleTypeSemanticAction($1, $3); }
	| SINGLE_TYPE OPEN_BRACKET CLOSE_BRACKET 																								{ $$ = ArrayTypeSemanticAction($1); }
	| SINGLE_TYPE OPEN_BRACE CLOSE_BRACE PIPE type																							{ $$ = SingleTypeSemanticAction($1, $5); }
	;

variableType: ID COLON type																													{ $$ = VariableTypeSemanticAction($1, $3); }
	| ID																																	{ $$ = VariableTypeSemanticAction($1, NULL); }
	;

constant: INT_VALUE																															{ $$ = IntConstantSemanticAction($1); }
	| FLOAT_VALUE																															{ $$ = FloatConstantSemanticAction($1); }
	| STRING_VALUE																															{ $$ = StringConstantSemanticAction($1); }
	| BOOL_VALUE																															{ $$ = BooleanConstantSemanticAction($1); }
	;
	
arrayContent: expression COLON arrayContent 																								{ $$ = ArrayContentSemanticAction($1, $3); }
	| expression 																															{ $$ = ArrayContentSemanticAction($1, NULL); }
	| %empty																																{ $$ = NULL; }
	;

objectContent: ID COLON expression 																											{ $$ = ObjectContentSemanticAction($1, $3, NULL); }
	| ID COLON expression COMA objectContent 																								{ $$ = ObjectContentSemanticAction($1, $3, $5); }
	;

promiseReturnType: PROMISE GREATER type LESS																								{ $$ = PromiseReturnTypeSemanticAction($3); }
	| %empty																																{ $$ = PromiseReturnTypeSemanticAction(NULL); }
	;

// Variable declaration and assignment -----------------------------------------------------------------------------------------------------------------
declaration: LET variableType ASSIGN expression																								{ $$ = DeclarationSemanticAction(LET_DT, $2, $4); }
	| LET variableType																														{ $$ = DeclarationSemanticAction(LET_DT, $2, NULL); }
	| LET variableType ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET																		{ $$ = DeclarationArraySemanticAction(LET_DT, $2, $5); }
	| LET variableType ASSIGN OPEN_BRACE objectContent CLOSE_BRACE																			{ $$ = DeclarationObjectSemanticAction(LET_DT, $2, $5); }
	| CONST variableType ASSIGN expression 								       																{ $$ = DeclarationSemanticAction(CONST_DT, $2, $4); }
	| CONST variableType ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET																		{ $$ = DeclarationArraySemanticAction(CONST_DT, $2, $5); }
	| VAR variableType ASSIGN expression																										{ $$ = DeclarationSemanticAction(VAR_DT, $2, $4); }
	| VAR variableType																														{ $$ = DeclarationSemanticAction(VAR_DT, $2, NULL); }
	| VAR variableType ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET																		{ $$ = DeclarationArraySemanticAction(VAR_DT, $2, $5); }
//	| declaration COMA variableType													{ } // VER
	;

typeDeclaration: TYPE ID ASSIGN objectContent																								{ $$ = ObjectTypeDeclarationSemanticAction($2, $4); }
	| TYPE ID ASSIGN expression																												{ $$ = VariableTypeDeclarationSemanticAction($2, $4); }
	| TYPE ID ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET 																				{ $$ = ArrayTypeDeclarationSemanticAction($2, $5); }
	| ENUM ID OPEN_BRACE argumentList CLOSE_BRACE 																							{ $$ = EnumTypeDeclarationSemanticAction($2,$4); }
	| INTERFACE ID OPEN_BRACE variableTypeList CLOSE_BRACE 																					{ $$ = InterfaceTypeDeclarationSemanticAction($2,$4); }
	;

variable: variableType ASSIGN expression																										{ $$ = VariableSemanticAction($1, $3); }
	| variableType ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET																			{ $$ = VariableArraySemanticAction($1, $4); }
	| variableType ASSIGN OPEN_BRACE objectContent CLOSE_BRACE																				{ $$ = VariableObjectSemanticAction($1, $4); }
	;

variableList: variable																														{ $$ = VariableListSemanticAction($1, NULL); }
	| variable COMA variableList  																											{ $$ = VariableListSemanticAction($1, $3); }
	| %empty																																{ $$ = NULL; }
	;

variableTypeList: variableType																												{ $$ = VariableTypeListSemanticAction($1, NULL); }
	| variableType COMA variableTypeList																									{ $$ = VariableTypeListSemanticAction($1, $3); }
	| %empty																																{ $$ = NULL; }
	;

argumentList: expression																													{ $$ = ArgumentListSemanticAction($1, NULL); }
	| expression COMA argumentList																											{ $$ = ArgumentListSemanticAction($1, $3); }
	| %empty																																{ $$ = NULL; }
	;

/* iterableVariable: ID  																														{ $$ = IterableVariableSemanticAction($1); }
	| functionCall 																															{ $$ = IterableVariableSemanticAction($1); }
	;  */

// if -----------------------------------------------------------------------------------------------------------------
ifStatement: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 													{ $$ = IfSemanticAction($3, $6, NULL); }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE ELSE OPEN_BRACE code CLOSE_BRACE 						{ $$ = IfSemanticAction($3, $6, $10); }
	;

//for -----------------------------------------------------------------------------------------------------------------
/* paramsFor: OPEN_PARENTHESIS declaration SEMI_COLON expression SEMI_COLON expression CLOSE_PARENTHESIS 											{ $$ = ParamsForSemanticAction($2, $4, $6, FOR_CLASSIC); }
	| OPEN_PARENTHESIS declaration OF iterableVariable CLOSE_PARENTHESIS 																	{ $$ = ParamsForSemanticAction($2, $5, NULL, FOR_OF); }
	;

forLoop: FOR paramsFor OPEN_BRACE code CLOSE_BRACE 																							{ $$ = ForSemanticAction($2, $4); }
	; */

//while -----------------------------------------------------------------------------------------------------------------
whileLoop: WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 													{ $$ = WhileSemanticAction($3, $6); }
	;

// await -----
//await: AWAIT expression 																													{ $$ = AwaitSemanticAction($2); }
//	;

// function -----------------------------------------------------------------------------------------------------------------
functionCall: ID OPEN_PARENTHESIS argumentList CLOSE_PARENTHESIS 																			{ $$ = FunctionCallSemanticAction($1,$3); }
	;

functionDeclaration: FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS COLON type OPEN_BRACE code CLOSE_BRACE 					{ $$ = FunctionDeclarationSemanticAction($2,$4,$7,$9); }
	;

arrowFunction: OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS ARROW COLON type OPEN_BRACE code CLOSE_BRACE 							{ $$ = ArrowFunctionSemanticAction($2,$6,$8); }
	;

asyncFunction: ASYNC FUNCTION ID OPEN_PARENTHESIS variableTypeList CLOSE_PARENTHESIS COLON promiseReturnType OPEN_BRACE code CLOSE_BRACE 	{ $$ = AsyncFunctionSemanticAction($3,$5,$8,$10); }
	;

%%
