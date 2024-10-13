#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum DeclarationType DeclarationType;
typedef enum AssignOperator AssignOperator;
typedef enum StatementType StatementType;
typedef enum DataType DataType;
typedef enum IncDecType IncDecType;
typedef enum IncDecPosition IncDecPosition;
typedef enum ForLoopType ForLoopType;
typedef enum IterableType IterableType;

typedef union Type Type;
typedef struct IterableVariable IterableVariable;

typedef struct Constant Constant;
typedef struct Array Array;
typedef struct ArrayContent ArrayContent;

typedef struct ObjectContent ObjectContent;
typedef struct Object Object;

typedef struct IncDec IncDec;
typedef struct Expression Expression;
typedef struct Factor Factor;

typedef struct Variable Variable;
typedef struct VariableType VariableType;
typedef struct Declaration Declaration;

typedef struct ArgumentList ArgumentList;
typedef struct VariableList VariableList;
typedef struct VariableTypeList VariableTypeList;
typedef struct VariableTypeList VariableTypeList;

typedef struct ParamsFor ParamsFor;
typedef struct ForLoop ForLoop;
typedef struct IfStatement IfStatement;
typedef struct WhileLoop WhileLoop;

typedef struct Interface Interface;
typedef struct Enum Enum;

typedef struct PromiseReturnType PromiseReturnType;

typedef struct FunctionCall FunctionCall;
typedef struct FunctionDeclaration FunctionDeclaration;
typedef struct ArrowFunction ArrowFunction;
typedef struct AsyncFunction AsyncFunction;

typedef struct Code Code;
typedef struct Program Program;
/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
	ADD_OP,
	SUB_OP,
	DIV_OP,
	MUL_OP,
	GREATER_OP,
	LESS_OP,
	EQUAL_OP,
	NEQUAL_OP,
	GREATER_EQUAL_OP,
	LESS_EQUAL_OP,
	STRICT_EQUAL_OP,
	STRICT_NEQUAL_OP,
	OR_OP,
	AND_OP,
	AWAIT_OP,
	NOT_OP,
	FACTOR
};

enum FactorType {
	CONSTANT,
	VARIABLE,
	EXPRESSION,
	FUNCTIONCALL
};

enum DeclarationType {
	LET_DT,
	CONST_DT,
	VAR_DT
};

enum AssignOperator {
	ASSIGNMENT,
	ASSIGNMENT_ADD,
	ASSIGNMENT_SUB,
	ASSIGNMENT_MUL,
	ASSIGNMENT_DIV
};

enum StatementType {
	IF_ST,
	WHILE_ST,
	FOR_ST,
	DECLARATION_ST,
	EXPRESSION_ST,
	VARIABLE_ST,
	FUNCTIONCALL_ST,
	FUNCTION_DECLARATION_ST,
	ARROW_FUNCTION_ST,
	ASYNC_FUNCTION_ST,
	ENUM_ST,
	INTERFACE_ST
};

enum DataType {
	NUMBER,
	STRING,
	BOOLEAN,
	ANY,
	UNDEFINED,
	VOID
};

enum ObjectType {
	NBR_ARRAY,
	STR_ARRAY,
	BOOL_ARRAY,
	ANY_ARRAY,
	UNDEFINED_ARRAY,
	OBJECT
};

enum specialType {
	ANY,
	UNKOWN,
	NEVER
};

union Type {
	DataType singleType;
	DataType *unionType;
};

struct Constant {
	union {
		int integer;
		float floating;
		char *string;
		char boolean;
	};
	DataType type;
};

struct VariableType {
	char *id;
	DataType type;
};

struct Variable {
	VariableType *variableType;
	Expression *value;
};

struct PromiseReturnType {
	Type *type;
};

struct Declaration {
	DeclarationType type;
	Variable *variable;
};

struct Factor {
	union {
		Constant *constant;
		char *variableName;
		Expression *expression; // ( expression )
		FunctionCall *functionCall;
	};
	FactorType type;
};

enum IncDecType {
	INC_OP,
	DEC_OP
};

enum IncDecPosition {
	PREFIX,
	POSTFIX
};

struct IncDec {
	Expression *expression;
	IncDecType type;
	IncDecPosition position;
};

struct Expression {
	union {
		Factor *factor;
		struct {
			Expression *leftExpression;
			Expression *rightExpression;
		};
		Expression *expression; // para expresiones unarias !, await
	};
	ExpressionType type;
};

struct FunctionCall {
	char *id;
	ArgumentList *arguments;
};

struct ArgumentList {
	Expression *expression;
	ArgumentList *next;
};

struct VariableTypeList {
	VariableType *variableType;
	struct variableTypeList *next;
};

struct Interface {
	char *id;
	VariableTypeList *variables;
};

struct VariableList {
	Variable *variable;
	struct variableList *next;
};

struct Enum {
	char *id;
	VariableList *values;
};

struct await {
	Expression *expression;
};

struct ArrayContent {
	Expression *value;
	ArrayContent *next;
};

struct Array {
	ArrayContent *arrayContent;
	char *id;
};

struct ObjectContent {
	char *key;
	Expression *value;
	ObjectContent *next;
};

struct Object {
	char *id;
	VariableList *variables;
};

struct IterableVariable {
	union {
		Array *array;
		Object *object;
	};
	IterableType type;
};

enum ForLoopType {
	FOR_CLASSIC,
	FOR_OF
};

struct ParamsFor {
	ForLoopType type;

	union {
		struct {				   // For classic
			Declaration *init;	   // Initialization
			Expression *condition; // condition
			Expression *update;	   // Update expression
		} forClassic;
		struct {				  // for...of
			Declaration *value;	  // value
			Expression *iterable; // Iterable object
		} forOf;
	};
};

struct ForLoop {
	ParamsFor *params;
	Code *body;
};

struct IfStatement {
	Expression *condition;
	Code *thenBody; // THEN code
	Code *elseBody; // ELSE code
};

struct WhileLoop {
	Expression *condition;
	Code *body;
};

struct FunctionDeclaration {
	char *id;
	VariableType *arguments;
	Type *returnType;
	Code *body;
};

struct ArrowFunction {
	VariableType *arguments;
	Type *returnType;
	Code *body;
};

struct AsyncFunction {
	char *id;
	VariableTypeList *arguments;
	Type *returnType;
	Code *body;
};

struct Code {
	StatementType statement;
	union {
		IfStatement *ifStatement;
		ForLoop *forLoop;
		WhileLoop *whileLoop;
		Declaration *declaration;
		Expression *expression;
		Variable *variable;
		FunctionCall *functionCall;
		FunctionDeclaration *FunctionDeclaration;
		ArrowFunction *arrowFunction;
		AsyncFunction *asyncFunction;
		Enum *enumm;
		Interface *interface;
	};
	struct Code *next;
};

struct Program {
	Code *code;
};

/**
 * Node recursive destructors.
 */
void releaseExpression(Expression *expression);
void releaseFactor(Factor *factor);
void releaseConstant(Constant *constant);

void releaseVariable(Variable *variable);
void releaseVariableType(VariableType *variableType);
void releaseVariableName(char *variableName);
void releaseVariableTypeList(VariableTypeList *variableTypeList);
void releaseVariableList(VariableList *variableList);
void releaseArgumentList(ArgumentList *argumentList);

void releaseEnum(Enum *enumm);
void releaseInterface(Interface *interface);

void releaseDeclaration(Declaration *declaration);

void releaseIf(IfStatement *ifStatement);
void releaseForParams(ParamsFor *params);
void releaseFor(ForLoop *forLoop);
void releaseWhile(WhileLoop *whileLoop);

void releaseFunctionCall(FunctionCall *functionCall);
void releaseFunctionDeclaration(FunctionDeclaration *functionDeclaration);
void releaseArrowFunction(ArrowFunction *arrowFunction);
void releaseAysncFunction(AsyncFunction *asyncFunction);

void releaseCode(Code *code);
void releaseProgram(Program *program);

#endif
