%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals */
	char * single_type;
	char * id;
	int valueInt;
    double valueFloat;
    char *valueStr;
    char valueBool;
	char * error;
	Token token;

	/** Non-terminals */
	Declaration * declaration; 

	Type * type;
	VariableType * variableType;
	Constant * constant;
	ArrayContent * arrayContent;
	Array * array;
	ObjectContent * objectContent;
	Object * object;

	VariableList * variableList;
	VariableTypeList * variableTypeList;
	ArgumentList * argumentList;
	Enum * enumm;
	Interface * interface;

	AssignOperator * assignOperator;
	Variable * variable;

	Expression * expression;
	ExpressionType * expressionType;
	IncDecType * incDecType;
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

%token <token> LET
%token <token> CONST
%token <token> VAR

%token <single_type> SINGLE_TYPE
%token <token> TYPE

%token <id> ID 
%token <token> COLON
%token <token> PIPE
%token <token> EQUAL
%token <valueInt> INT_VALUE
%token <valueFloat> FLOAT_VALUE
%token <valueStr> STRING_VALUE
%token <valueBool> BOOL_VALUE

%token <token> SEMI_COLON 
%token <token> COMA
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> DOT

%token <token> INCREMENT
%token <token> DECREMENT

%token <token> ADD
%token <token> DIV
%token <token> MUL
%token <token> SUB

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

%token <token> ADD_ASSIGN
%token <token> SUB_ASSIGN
%token <token> MUL_ASSIGN
%token <token> DIV_ASSIGN

%token <token> ARROW
%token <token> AYSNC
%token <token> PROMISE
%token <token> AWAIT


/** Non-terminals. */
%type <declaration> declaration
%type <type> type

%type <variableType> variableType
%type <constant> constant
%type <arrayContent> arrayContent
%type <array> array
%type <objectContent> objectContent
%type <object> object

%type <argumentList> argumentList
%type <variableList> variableList
%type <variableTypeList> variableTypeList


%type <variable> variable
%type <assignOperator> assignOperator

%type <enumm> enumm
%type <interface> interface

%type <expressionType> expressionType
%type <incDecType> incDecType
%type <incDec> incDec
%type <expression> expression

%type <promiseReturnType> promiseReturnType
%type <await> await

%type <ifStatement> ifStatement
%type <forLoop> forLoop
%type <whileLoop> whileLoop

%type <functionDeclaration> functionDeclaration
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

// Expression -----------------------------------------------------------------------------------------------------------------
expressionType: ADD         																											{ $$ = ADD_OP; }
	| SUB      																															{ $$ = SUB_OP; }
	| MUL 																																{ $$ = MUL_OP; }
	| DIV 																																{ $$ = DIV_OP; }
	| GREATER       																													{ $$ = GREATER_OP; }
    | LESS         																														{ $$ = LESS_OP; }
    | NEQUAL       																														{ $$ = NEQUAL_OP; }
    | STRICT_EQUAL																														{ $$ = STRICT_EQUAL_OP; }
    | STRICT_NEQUAL 																													{ $$ = STRICT_NEQUAL_OP; }
    | LESS_EQUAL   																														{ $$ = LESS_EQUAL_OP; }
    | GREATER_EQUAL 																													{ $$ = GREATER_EQUAL_OP; }
    | OR 																																{ $$ = OR_OP; }
	| AND 																																{ $$ = AND_OP; }
	;

incDecType: INCREMENT 																													{ $$ = INC_OP; }
	| DECREMENT 																														{ $$ = DEC_OP; }																			
	;

incDec: ID incDecType 																													{ $$ = 	IncDecSemanticAction($1,$2, POSFIX); }
	| OPEN_PARENTHESIS incDec CLOSE_PARENTHESIS incDecType																				{ $$ = 	IncDecSemanticAction($1,$3, POSFIX); }
	| incDecType ID 																													{ $$ =  IncDecSemanticAction($2,$1, PREFIX); }
	| incDecType OPEN_PARENTHESIS incDec CLOSE_PARENTHESIS																				{ $$ =  IncDecSemanticAction($3,$1, PREFIX); }
	;

expression: expression[left] expressionType[op] expression[right]																		{ $$ = ExpressionSemanticAction($left, $right, $op); }
	| NOT expression 																													{ $$ = ExpressionSemanticAction($2, NULL, NOT_OP);}
	| factor																															{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS																					{ $$ = ExpressionFactorSemanticAction($2); }
	| ID																													    		{ $$ = VariableFactorSemanticAction($1); }
	| constant 																															{ $$ = ValueFactorSemanticAction($1); }
	| functionCall 																														{ $$ = FunctionFactorSemanticAction($1) }
	;

// Type -------------------------------------------------------------------------------------------------------------------------------------------------
type: SINGLE_TYPE																    														{ $$ = SingleTypeSemanticAction($1); }
	| SINGLE_TYPE PIPE type																													{ $$ = UnionTypeSemanticAction($1, $3); }
	| type OPEN_BRACKET CLOSE_BRACKET 																										{ $$ = ArrayTypeSemanticAction($1); }
	;

variableType: ID COLON type																													{ $$ = VariableTypeSemanticAction($1, $3); }
	| ID																																	{ $$ = VariableTypeSemanticAction($1, NULL); } // CHECK
	;

constant: INT_VALUE																															{ $$ = IntConstantSemanticAction($1); }
	| FLOAT_VALUE																															{ $$ = FloatConstantSemanticAction($1); }
	| STRING_VALUE																															{ $$ = StringConstantSemanticAction($1); }
	| BOOL_VALUE																															{ $$ = BoolConstantSemanticAction($1); }
	;

array: OPEN_BRACKET arrayContent CLOSE_BRACKET																								{$$ = ArraySemanticAction($2); }
	;
	
arrayContent: expression COLON arrayContent 																								{ $$ = ArrayContentSemanticAction($1, $3); }
	| expression 																															{ $$ = ArrayContentSemanticAction($1, NULL); }
	| %empty	
	;

object: OPEN_BRACE objectContent CLOSE_BRACE 																								{ $$ = ObjectSemanticAction($2); }
	;

objectContent: ID COLON expression 																											{ $$ = ObjectContentSemanticAction($1, $3); }
	| ID COLON expression COMA objectContent 																								{ $$ = ObjectContentSemanticAction($1, $3, $5); }
	;

inicializeObject: ID COLON expression 																										{ $$ = InicializeObjectSemanticAction($1, $3); }
	| ID COLON expression COMA inicializeObject 																							{ $$ = InicializeObjectSemanticAction($1, $3, $5); }
	;

promiseReturnType: PROMISE GREATER type LESS																								{ $$ = PromiseReturnTypeSemanticAction($2); }
	| %empty																																{ $$ = PromiseReturnTypeSemanticAction(NULL); }
	;

// Variable declaration and assignment -----------------------------------------------------------------------------------------------------------------
declaration: LET variableType EQUAL expression																								{ $$ = DeclarationSemanticAction(LET_DT, $2, $4); }
	| LET variableType																														{ $$ = DeclarationSemanticAction(LET_DT, $2, NULL); }
	| CONST variableType EQUAL expression 								       																{ $$ = DeclarationSemanticAction(CONST_DT, $2, $4); }
	| VAR variableType EQUAL expression																										{ $$ = DeclarationSemanticAction(VAR_DT, $2, $4); }
	| VAR variableType																														{ $$ = DeclarationSemanticAction(VAR_DT, $2, NULL); }
//	| declaration COMA variableType													{ } // VER
	;
	
typeDeclaration: TYPE ID EQUAL object																										{ $$ = ObjectTypeDeclarationSemanticAction($2, $4); }																								{ $$ = TypeDeclarationSemanticAction($2, $4); }
	| TYPE ID EQUAL expression																												{ $$ = AnyTypeDeclarationSemanticAction($2, $4); }
	;																																		{ $$ = DeclarationObjectSemanticAction($2, $4); }

assignOperator: EQUAL 															    														{ $$ = ASSIGNMENT; }
	| ADD_ASSIGN 																															{ $$ = ASSIGNMENT_ADD; }
	| SUB_ASSIGN 																															{ $$ = ASSIGNMENT_SUB; } 
	| MUL_ASSIGN 																															{ $$ = ASSIGNMENT_MUL; }
	| DIV_ASSIGN 																															{ $$ = ASSIGNMENT_DIV; }
	;

variable: variableType assignOperator[op] expression																						{ $$ = AssignOperatorAssignSemanticAction($1, $2, $3); }
	;

variableList: variable																														{ $$ = VariableSemanticAction($1); }
	| variable COMA variableList  																											{ $$ = MultipleAssignListSemanticAction($1, $3); }
	| variable COMA variableList																											{ $$ = MultipleAssignListSemanticAction($1, $3); }
	;

variableTypeList: variableType																												{ $$ = VariableTypeListSemanticAction($1, NULL); }
	| variableType COMA variableTypeList																									{ $$ = VariableTypeListSemanticAction($1, $2); }
	;

argumentList: expression																													{ $$ = ArgumentListSemanticAction($1, NULL); }
	| expression COMA argumentList																											{ $$ = ArgumentListSemanticAction($1, $3); }
	;

/* iterableVariable: ID  																														{ $$ = IterableVariableSemanticAction($1); }
	| functionCall 																															{ $$ = IterableVariableSemanticAction($1); }
	; */

// enum and Interface -----------------------------------------------------------------------------------------------------------------
enumm: ENUM ID OPEN_BRACE argumentList CLOSE_BRACE																							{ $$ = EnumSemanticAction($2, $4); }
	;

interface: INTERFACE ID OPEN_BRACE variableTypeList CLOSE_BRACE																				{ $$ = InterfaceSemanticAction($2, $4); }
	;

// if -----------------------------------------------------------------------------------------------------------------
ifStatement: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 													{ $$ = IfSemanticAction($3, $6, NULL); }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE ELSE OPEN_BRACE code CLOSE_BRACE 						{ $$ = IfSemanticAction($3, $6, $10); }
	;

//for -----------------------------------------------------------------------------------------------------------------
paramsFor: OPEN_PARENTHESIS declaration SEMI_COLON expression SEMI_COLON expression CLOSE_PARENTHESIS 											{ $$ = ParamsForSemanticAction($2, $4, $6, FOR_CLASSIC); }
	//| OPEN_PARENTHESIS declaration OF iterableVariable CLOSE_PARENTHESIS 																	{ $$ = ParamsForSemanticAction($2, $5, NULL, FOR_OF); }
	;

forLoop: FOR paramsFor OPEN_BRACE code CLOSE_BRACE 																							{ $$ = ForSemanticAction($2, $4); }
	;

//while -----------------------------------------------------------------------------------------------------------------
whileLoop: WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS OPEN_BRACE code CLOSE_BRACE 													{ $$ = WhileSemanticAction($3, $6); }
	;

// await -----
await: AWAIT expression 																													{ $$ = AwaitSemanticAction($2); }
	;

// function -----------------------------------------------------------------------------------------------------------------
functionCall: ID OPEN_PARENTHESIS argumentList CLOSE_PARENTHESIS 																			{ $$ = functionCallSemanticAction($1,$3); }
	;

functionDeclaration: FUNCTION ID OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS COLON type OPEN_BRACE code CLOSE_BRACE 					{ $$ = FunctionDeclarationSemanticAction($2,$4,$7,$8)}
	;

arrowFunction: OPEN_PARENTHESIS variableList CLOSE_PARENTHESIS ARROW COLON type OPEN_BRACE code CLOSE_BRACE 							{ $$ = ArrowFunctionSemanticAction($2,$6,$8); }
	;

asyncFunction: AYSNC FUNCTION ID OPEN_PARENTHESIS variableTypeList CLOSE_PARENTHESIS COLON promiseReturnType OPEN_BRACE code CLOSE_BRACE 	{ $$ = AsyncFunctionSemanticAction($3,$5,$8,$10); }
	;

// Code -----------------------------------------------------------------------------------------------------------------
code: ifStatement code 																		 												{ $$ = IfCodeSemanticAction($1, $2); }
	| forLoop code 																		 													{ $$ = ForCodeSemanticAction($1, $2); }
	| whileLoop code 																	 													{ $$ = WhileCodeSemanticAction($1, $2); }
	| declaration code															 	 														{ $$ = DeclarationCodeSemanticAction($1, $2); }
	| enumm code 																	 														{ $$ = enummCodeSemanticAction($1, $2); }
	| interface code															     														{ $$ = InterfaceCodeSemanticAction($1, $2); }
	| functionCall code 															 														{ $$ = FunctionaCallCodeSemanticAction($1, $2); }
	| functionDeclaration code 																 												{ $$ = FunctionDeclarationCodeSemanticAction($1, $2); }
	| arrowFunction code 															 														{ $$ = ArrowFunctionCodeSemanticAction($1, $2); }
	| asyncFunction code 															 														{ $$ = AsyncFunctionCodeSemanticAction($1, $2); }
	| expression code 															 															{ $$ = ExpressionCodeSemanticAction($1, $2); }
	| incDec code 																															{ $$ = IncDecCodeSemanticAction($1, $2); }
	| %empty																																{ $$ = NULL; }
	;

program: code 																																{ $$ = CodeProgramSemanticAction(currentCompilerState(),$1); }
	;

%%
