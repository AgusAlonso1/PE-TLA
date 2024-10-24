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
	Program * program;
	Code * code;
	Statement * statement;
	Declaration * declaration;
	TypeDeclaration * typeDeclaration;
	Type * type;
	VariableType * variableType;
	Constant * constant;
	ArrayContent * arrayContent;
	ObjectContent * objectContent;
	VariableTypeList * variableTypeList;
	ArgumentList * argumentList;
	IterableVariable * iterableVariable;
	Variable * variable;
	Expression * expression;
	IncDec * incDec;
	Factor * factor;
	IfStatement *ifStatement;
	WhileLoop * whileLoop;
	ParamsFor * paramsFor;
	ForLoop * forLoop;
	SwitchContent * switchContent;
	SwitchStatement * switchStatement;
	PromiseReturnType * promiseReturnType;
	ReturnValue * returnValue;
	FunctionBody * functionBody;
	FunctionDeclaration * functionDeclaration; 
	//ArrowFunction * arrowFunction;
	AsyncFunction * asyncFunction;
	//Await * await;
	FunctionCall * functionCall;

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
%token <token> SWITCH
%token <token> CASE
%token <token> BREAK
%token <token> DEFAULT

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
%type <program> program
%type <code> code
%type <statement> statement

%type <incDec> incDec
%type <expression> expression
%type <factor> factor

%type <type> type
%type <variableType> variableType
%type <constant> constant
%type <arrayContent> arrayContent
%type <objectContent> objectContent

%type <declaration> declaration
%type <typeDeclaration> typeDeclaration
%type <variable> variable
%type <variableTypeList> variableTypeList
%type <argumentList> argumentList
%type <iterableVariable> iterableVariable

%type <ifStatement> ifStatement
%type <whileLoop> whileLoop
%type <paramsFor> paramsFor
%type <forLoop> forLoop
%type <switchContent> switchContent
%type <switchStatement> switchStatement

//%type <await> await
%type <returnValue> returnValue
%type <promiseReturnType> promiseReturnType

%type <functionBody> functionBody
%type <functionDeclaration> functionDeclaration
%type <functionCall> functionCall
%type <asyncFunction> asyncFunction
//%type <arrowFunction> arrowFunction

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%nonassoc IF
%nonassoc ELSE
%nonassoc CLOSE_BRACE
%nonassoc ARROW

%left ADD SUB
%left MUL DIV
%left INCREMENT DECREMENT
%left GREATER LESS NEQUAL STRICT_EQUAL STRICT_NEQUAL LESS_EQUAL GREATER_EQUAL EQUAL
%left AND 
%left OR
%right NOT
%right ASSIGN


%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: code 																															{ $$ = CodeProgramSemanticAction(currentCompilerState(),$1); }
	;

// Code -----------------------------------------------------------------------------------------------------------------
code: statement 																														{ $$ = CodeSemanticAction($1, NULL); }
	| statement code																													{ $$ = CodeSemanticAction($1, $2); }
	;

statement: declaration 															 	 													{ $$ = DeclarationStatementSemanticAction($1); }
	| typeDeclaration  																     												{ $$ = TypeDeclarationStatementSemanticAction($1); }
	| ifStatement  																														{ $$ = IfStatementSemanticAction($1); }
	| whileLoop  																														{ $$ = WhileStatementSemanticAction($1); }
	| forLoop  																															{ $$ = ForStatementSemanticAction($1); }
	| switchStatement  																													{ $$ = SwitchStatementSemanticAction($1); }
	| functionDeclaration  																												{ $$ = FunctionDeclarationStatementSemanticAction($1); }
	| functionCall																														{ $$ = FunctionCallStatementSemanticAction($1); }
	| asyncFunction  																													{ $$ = AsyncFunctionStatementSemanticAction($1); }
	| variable																															{ $$ = VariableStatementSemanticAction($1); }
	;

// Expression -----------------------------------------------------------------------------------------------------------------
incDec: INCREMENT expression																											{ $$ = IncDecSemanticAction($2, INC_OP, PREFIX); }
    | DECREMENT expression																												{ $$ = IncDecSemanticAction($2, DEC_OP, PREFIX); }
	| expression INCREMENT																												{ $$ = IncDecSemanticAction($1, INC_OP, POSTFIX); }
	| expression DECREMENT																												{ $$ = IncDecSemanticAction($1, DEC_OP, POSTFIX); }
    ;

expression: factor																														{ $$ = FactorExpressionSemanticAction($1); }
	| expression[left] ADD expression[right] 																							{ $$ = ExpressionSemanticAction($left, $right, ADD_OP); }
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
	| NOT expression 																													{ $$ = ExpressionSemanticAction($2, NULL, NOT_OP); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																					{ $$ = ExpressionFactorSemanticAction($2); }
	| constant 																															{ $$ = ConstantFactorSemanticAction($1); }
	| ID																													    		{ $$ = VariableFactorSemanticAction($1); }
	;

// Type -------------------------------------------------------------------------------------------------------------------------------------------------
type: SINGLE_TYPE																    														{ $$ = SingleTypeSemanticAction($1, NULL); }
	| SINGLE_TYPE PIPE type																													{ $$ = SingleTypeSemanticAction($1, $3); }
	| SINGLE_TYPE OPEN_BRACKET CLOSE_BRACKET 																								{ $$ = ArrayTypeSemanticAction($1); }
	| SINGLE_TYPE OPEN_BRACKET CLOSE_BRACKET PIPE type																							{ $$ = SingleTypeSemanticAction($1, $5); }
	;

variableType: ID COLON type																													{ $$ = VariableTypeSemanticAction($1, $3); }
	| ID																																	{ $$ = VariableTypeSemanticAction($1, NULL); }
	;

constant: INT_VALUE																															{ $$ = IntConstantSemanticAction($1); }
	| FLOAT_VALUE																															{ $$ = FloatConstantSemanticAction($1); }
	| STRING_VALUE																															{ $$ = StringConstantSemanticAction($1); }
	| BOOL_VALUE																															{ $$ = BooleanConstantSemanticAction($1); }
	;
	
arrayContent: expression COMA arrayContent 																									{ $$ = ArrayContentSemanticAction($1, $3); }
	| expression 																															{ $$ = ArrayContentSemanticAction($1, NULL); }
	| %empty																																{ $$ = NULL; }
	;

objectContent: ID COLON expression 																											{ $$ = ObjectContentSemanticAction($1, $3, NULL); }
	| ID COLON expression COMA objectContent 																								{ $$ = ObjectContentSemanticAction($1, $3, $5); }
	;

// Variable declaration and assignment -----------------------------------------------------------------------------------------------------------------
declaration: LET variable																													{ $$ = DeclarationVariableSemanticAction(LET_DT, $2); }
	| LET variableType																														{ $$ = DeclarationSemanticAction(LET_DT, $2); }
	| CONST variable 								       																					{ $$ = DeclarationVariableSemanticAction(CONST_DT, $2); }
	| VAR variable																															{ $$ = DeclarationVariableSemanticAction(VAR_DT, $2); }
	| VAR variableType																														{ $$ = DeclarationSemanticAction(VAR_DT, $2); }
	;

typeDeclaration: TYPE ID ASSIGN OPEN_BRACE objectContent CLOSE_BRACE																		{ $$ = ObjectTypeDeclarationSemanticAction($2, $5); }
	| TYPE ID ASSIGN expression																												{ $$ = VariableTypeDeclarationSemanticAction($2, $4); }
	| TYPE ID ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET 																				{ $$ = ArrayTypeDeclarationSemanticAction($2, $5); }
	| ENUM ID OPEN_BRACE argumentList CLOSE_BRACE 																							{ $$ = EnumTypeDeclarationSemanticAction($2,$4); }
	| INTERFACE ID OPEN_BRACE variableTypeList CLOSE_BRACE 																					{ $$ = InterfaceTypeDeclarationSemanticAction($2,$4); }
	;

variable: variableType ASSIGN expression																									{ $$ = VariableExpressionSemanticAction($1, $3); }
	| variableType ASSIGN OPEN_BRACKET arrayContent CLOSE_BRACKET																			{ $$ = VariableArraySemanticAction($1, $4); }
	| variableType ASSIGN OPEN_BRACE objectContent CLOSE_BRACE																				{ $$ = VariableObjectSemanticAction($1, $4); }
	| variableType ASSIGN functionCall																										{ $$ = VariableFunctionCallSemanticAction($1,$3); }
	//| variableType ASSIGN arrowFunction																										{ $$ = VariableArrowFunctionSemanticAction($1, $3); }
	;

variableTypeList: variableType																												{ $$ = VariableTypeListSemanticAction($1, NULL); }
	| variableType COMA variableTypeList																									{ $$ = VariableTypeListSemanticAction($1, $3); }
	| %empty																																{ $$ = NULL; }
	;

argumentList: expression																													{ $$ = ArgumentListSemanticAction($1, NULL); }
	| expression COMA argumentList																											{ $$ = ArgumentListSemanticAction($1, $3); }
	| %empty																																{ $$ = NULL; }
	;

iterableVariable: ID																														{ $$ = IterableVariableNameSemanticAction($1); }
	| OPEN_BRACKET arrayContent	CLOSE_BRACKET																								{ $$ = IterableVariableArraySemanticAction($2); }
	| OPEN_BRACE objectContent	CLOSE_BRACE																									{ $$ = IterableVariableObjectContentSemanticAction($2); }
	| functionCall																															{ $$ = IterableVariableFunctionCallSemanticAction($1); }
	;

// Control structures -----------------------------------------------------------------------------------------------------------------
ifStatement: 
	IF OPEN_PARENTHESIS expression[condition] CLOSE_PARENTHESIS OPEN_BRACE code[then] CLOSE_BRACE ELSE OPEN_BRACE code[else] %prec CLOSE_BRACE CLOSE_BRACE				{ $$ = IfSemanticAction($condition, $then, $else); }
	| IF OPEN_PARENTHESIS expression[condition] CLOSE_PARENTHESIS OPEN_BRACE code[then] CLOSE_BRACE																		{ $$ = IfSemanticAction($condition, $then, NULL); }
	;

whileLoop: WHILE OPEN_PARENTHESIS expression[condition] CLOSE_PARENTHESIS OPEN_BRACE code[body] CLOSE_BRACE 								{ $$ = WhileSemanticAction($condition, $body); }
	;

paramsFor: declaration SEMI_COLON expression SEMI_COLON incDec 																			{ $$ = ForParamsIncDecSemanticAction($1, $3, $5); }
	| declaration SEMI_COLON expression SEMI_COLON expression 																			{ $$ = ForParamsSemanticAction($1, $3, $5); }
	| declaration SEMI_COLON expression SEMI_COLON 																						{ $$ = ForParamsSemanticAction($1, $3, NULL); }
	| SEMI_COLON expression SEMI_COLON incDec 																							{ $$ = ForParamsIncDecSemanticAction(NULL, $2, $4); }
	| SEMI_COLON expression SEMI_COLON expression 																						{ $$ = ForParamsSemanticAction(NULL, $2, $4); }
	| SEMI_COLON expression SEMI_COLON 																									{ $$ = ForParamsSemanticAction(NULL, $2, NULL); }
	| declaration OF iterableVariable 																									{ $$ = ForOfParamsSemanticAction($1, $3); }
	;

forLoop: FOR OPEN_PARENTHESIS paramsFor CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 													{ $$ = ForSemanticAction($3, $6); }
	;

switchContent: CASE expression[cond] COLON code[body] BREAK switchContent[next] 														{ $$ = SwitchContentSemanticAction($cond, $body, $next); }
	| DEFAULT COLON code[body] 																											{ $$ = SwitchContentSemanticAction(NULL, $body, NULL); }
	| %empty																															{ $$ = NULL; }
	;

switchStatement: SWITCH OPEN_PARENTHESIS ID[id] CLOSE_PARENTHESIS OPEN_BRACE switchContent[body] CLOSE_BRACE 							{ $$ = SwitchSemanticAction($id, $body); }
	;

// return type -----------------------------------------------------------------------------------------------------------------
/* await: AWAIT functionCall																													{ $$ = AwaitSemanticAction($2); }
	| AWAIT arrowFunction																													{ $$ = AwaitArrowFunctionSemanticAction($2); }
	| AWAIT asyncFunction																													{ $$ = AwaitAsyncFunctionSemanticAction($2); }
	; */
returnValue: RETURN expression 																												{ $$ = ReturnExpressionSemanticAction($2); }
	//| RETURN arrowFunction 																													{ $$ = ReturnArrowFunctionSemanticAction($2); }
	| RETURN functionCall 																													{ $$ = ReturnFunctionCallSemanticAction($2); }
	//| RETURN await
	| RETURN asyncFunction 																													{ $$ = ReturnAsyncFunctionSemanticAction($2); }
	| RETURN																																{ $$ = NULL; }
	;

promiseReturnType: COLON PROMISE OPEN_PARENTHESIS type CLOSE_PARENTHESIS																	{ $$ = PromiseReturnTypeSemanticAction($4); }
	| %empty																																{ $$ = NULL; }
	;
// Functions -----------------------------------------------------------------------------------------------------------------
functionBody: code returnValue																												{ $$ = FunctionBodySemanticAction($1, $2); }
	| code																																	{ $$ = FunctionBodySemanticAction($1, NULL); }
	| returnValue																															{ $$ = FunctionBodySemanticAction(NULL, $1); }
	;

functionDeclaration: FUNCTION ID[id] OPEN_PARENTHESIS variableTypeList[arg] CLOSE_PARENTHESIS OPEN_BRACE functionBody[body] %prec CLOSE_BRACE CLOSE_BRACE 								{ $$ = FunctionDeclarationSemanticAction($id, $arg, NULL, $body); }
	| FUNCTION ID[id] OPEN_PARENTHESIS variableTypeList[arg] CLOSE_PARENTHESIS COLON type[return] OPEN_BRACE functionBody[body] %prec CLOSE_BRACE CLOSE_BRACE 							{ $$ = FunctionDeclarationSemanticAction($id, $arg, $return, $body); }
	;

functionCall: ID[id] OPEN_PARENTHESIS argumentList[arg] CLOSE_PARENTHESIS	%prec CLOSE_PARENTHESIS																											{ $$ = FunctionCallSemanticAction($id, $arg, CLASSIC_CALL); }
	| AWAIT ID[id] OPEN_PARENTHESIS argumentList[arg] CLOSE_PARENTHESIS		%prec CLOSE_PARENTHESIS																											{ $$ = FunctionCallSemanticAction($id, $arg, AWAIT_CALL); }
	;

asyncFunction: ASYNC FUNCTION ID[id] OPEN_PARENTHESIS variableTypeList[arg] CLOSE_PARENTHESIS promiseReturnType[promise] OPEN_BRACE functionBody[body] %prec CLOSE_BRACE CLOSE_BRACE 	{ $$ = AsyncFunctionSemanticAction($id, $arg, $promise, $body); }
	;

/* arrowFunction: OPEN_PARENTHESIS variableTypeList[arg] CLOSE_PARENTHESIS COLON type[return] ARROW OPEN_BRACE functionBody[body] CLOSE_BRACE 	%prec ARROW							{ $$ = ArrowFunctionSemanticAction($arg, $return, $body); }
	| OPEN_PARENTHESIS variableTypeList[arg] CLOSE_PARENTHESIS ARROW OPEN_BRACE functionBody[body] CLOSE_BRACE 	%prec ARROW														{ $$ = ArrowFunctionSemanticAction($arg, NULL, $body); }
	; */


%%
