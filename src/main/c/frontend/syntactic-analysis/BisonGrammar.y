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
	AssignList * assignList;
	VarList * varList;
	Enum * enum;

	Program * program;

	AssignOperator * assignOperator;
	Assign * assign;

	ArithmeticExpression * arithmeticExpression;
	AritmeticOperator * arithmeticOperator;
	
	LogicalExpression * logicalExpression;
	LogicalOperator * logicalOperator;

	ComparisonExpression * comparisonExpression;
	ComparisonOperator * comparisonOperator;

	ReturnType * returnType;
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

%token <token> LET
%token <token> CONST
%token <token> VAR

%token <single_type> SINGLE_TYPE

%token <id> ID 
%token <token> COLON
%token <token> UNION
%token <token> EQUAL
%token <value> VAL

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



/** Non-terminals. */
%type <type> type
%type <assign> assign
%type <assignOperator> assignOperator
%type <variableType> variableType
%type <declaration> declaration
%type <interface> interface
%type <assignList> assignList
%type <varList> varList
%type <returnType> returnType
%type <enum> enum
%type <arithmeticExpression> arithmeticExpression
%type <arithmeticOperator> arithmeticOperator
%type <logicalExpression> logicalExpression
%type <logicalOperator> logicalOperator
%type <comparisonExpression> comparisonExpression
%type <comparisonOperator> comparisonOperator
%type <program> program



/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: NOSE													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;


arithmeticExpression: arithmeticExpression[left] ADD arithmeticExpression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| arithmeticExpression[left] DIV arithmeticExpression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| arithmeticExpression[left] MUL arithmeticExpression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| arithmeticExpression[left] SUB arithmeticExpression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| OPEN_PARENTHESIS arithmeticExpression CLOSE_PARENTHESIS					{ $$ = FactorSemanticAction($2); }
	;

logicalExpression:
    logicalExpression[left] OR logicalExpression[right]  { $$ = LogicalExpressionSemanticAction($left, $right, OR); }
    | logicalExpression[left] AND logicalExpression[right] { $$ = LogicalExpressionSemanticAction($left, $right, AND); }
    | comparisonExpression { $$ = $1; }
    ;

comparisonExpression: OPEN_PARENTHESIS arithmeticExpression comparisonOperator arithmeticExpression CLOSE_PARENTHESIS					{ $$ = FactorSemanticAction($2); } // CREO QUE SON SOLO NUMBERS VER 
	;


comparisonOperator:
    GREATER        { $$ = GREATER_OP; }
    | LESS         { $$ = LESS_OP; }
    | NEQUAL       { $$ = NEQUAL_OP; }
    | STRICT_EQUAL { $$ = STRICT_EQUAL_OP; }
    | STRICT_NEQUAL { $$ = STRICT_NEQUAL_OP; }
    | LESS_EQUAL   { $$ = LESS_EQUAL_OP; }
    | GREATER_EQUAL { $$ = GREATER_EQUAL_OP; }
    ;

logicalOperator:
	OR { $$ = OR_OP; }
	| AND { $$ = AND_OP; }
	;

arithmeticOperator:
	ADD { $$ = ADD_OP; }
	| SUB { $$ = SUB_OP; }
	| MUL { $$ = MUL_OP; }
	| DIV { $$ = DIV_OP; }
	;

assignOperator:
	ADD_ASSIGN { $$ = ADD_ASSIGN_OP; }
	| SUB_ASSIGN { $$ = SUB_ASSIGN_OP; }
	| MUL_ASSIGN { $$ = MUL_ASSIGN_OP; }
	| DIV_ASSIGN { $$ = DIV_ASSIGN_OP; }
	;

type: SINGLE_TYPE															{ $$ = SingleTypeSemanticAction($1); }
	| SINGLE_TYPE UNION type												{ $$ = UnionTypeSemanticAction($1, $3); }
	;

variableType: ID COLON type													{ $$ = VariableSemanticAction($1, $3); }
	;

returnType: COLON type														{ $$ = ReturnTypeSemanticAction($2); } // debe haber mas casos 
	;

declaration: LET variableType assign										{ $$ = LetDeclarationSemanticAction($2, $4); }
	| CONST variableType EQUAL VAL 							       			{ $$ = ConstDeclarationSemanticAction($2,$4); }
	| VAR variableType assign												{ $$ = VarDeclarationSemanticAction($2, $4); }
	| declaration COMA variableType											{ $$ = MultipleDeclarationSemanticAction($1, $2); } // pensar mejor  VER
	;

assign: EQUAL VAL															{ $$ = EqualAssignSemanticAction($2); }
	| assignOperator VAL													{ $$ = AssignOperatorAssignSemanticAction($1, $2); } // creo que solo aplica a number VER 
	;

assignList: variableType assign												{ $$ = AssignListSemanticAction($1); }
	| assignList COMA variableType assign									{ $$ = MultipleAssignListSemanticAction($1, $3); }
	| assignList COMA variableType											{ $$ = MultipleAssignListSemanticAction($1, $3); }
	;

varList: variableType														{ $$ = InterfaceListSemanticAction($1); }
	| varList COMA variableType												{ $$ = MultipleInterfaceListSemanticAction($1, $2); }
	;

enum: ENUM ID OPEN_BRACE assignList CLOSE_BRACE								{ $$ = EnumSemanticAction($2, $4); }
	;

interface: INTERFACE ID OPEN_BRACE declaration CLOSE_BRACE					{ $$ = InterfaceSemanticAction($2, $4); }
	;

if: IF OPEN_PARENTHESIS logicalExpression CLOSE_PARENTHESIS OPEN_BRACE program CLOSE_BRACE
	| IF OPEN_PARENTHESIS logicalExpression CLOSE_PARENTHESIS OPEN_BRACE program CLOSE_BRACE ELSE OPEN_BRACE program CLOSE_BRACE
	;

for: FOR OPEN_PARENTHESIS declaration SEMI_COLON logicalExpression SEMI_COLON assign CLOSE_PARENTHESIS OPEN_BRACE program CLOSE_BRACE

function: FUNCTION ID OPEN_PARENTHESIS varList CLOSE_PARENTHESIS returnType OPEN_BRACE program CLOSE_BRACE
	;
%%
