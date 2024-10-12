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
typedef enum StatementType StatementType;
typedef enum DataType DataType;
typedef struct Constant Constant;
typedef enum IncDecType IncDecType;
typedef enum IncDecPosition IncDecPosition;
typedef struct IncDec IncDec;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef union Type Type;
typedef struct Variable Variable;
typedef struct PromiseReturn PromiseReturn;
typedef struct Declaration Declaration;
typedef struct FunctionCall FunctionCall;
typedef struct ValueList ValueList;
typedef struct VariableType VariableType;
typedef enum ForLoopType ForLoopType;
typedef struct ParamsFor ParamsFor;
typedef struct ForLoop ForLoop;
typedef struct IfStatement IfStatement;
typedef struct WhileLoop WhileLoop;
typedef struct FunctionDeclaration FunctionDeclaration;
typedef struct Code Code;
typedef struct Program Program;
typedef struct Interface Interface;
typedef struct VariableTypeList VariableTypeList; //
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
	FUNCTIONCALL
};

enum DeclarationType {
	LET_DT,
	CONST_DT,
	VAR_DT
};

enum StatementType {
	IF,
	WHILE,
	FOR,
	DECLARATION,
	EXPRESSION,
	VARIABLE,
	FUNCTIONCALL
};

enum DataType {
	NUMBER,
	STRING,
	BOOLEAN,
	ANY,
	UNDEFINED,
	VOID,
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

struct PromiseReturn {
	Type *type;
};

struct Declaration {
	DeclarationType type;
	Variable *variable;
};

struct Factor {
	union {
		Constant *constant;
		Variable *variable;
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
	ValueList *arguments;
};

struct VariableTypeList {
	VariableType *variable;
	VariableTypeList *next;
};

struct ValueList {
	Expression *expression;
	ValueList *next;
};

struct await {
	Expression *expression;
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
	VariableType *params;
	DataType returnType;
	Code *body;
};

struct Code {
	StatementType statement;
	union {
		IfStatement ifStatement;
		ForLoop forLoop;
		WhileLoop whileLoop;
		Declaration declaration;
		Expression expression;
		Variable variable;
		FunctionCall functionCall;
		FunctionDeclaration FunctionDeclaration;
	};
	struct Code *next;
};

struct Interface {
	char *id;
	variableList *variables;
};

typedef struct Program {
	Code *code;
} Program;

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant *constant);
void releaseExpression(Expression *expression);
// void releaseFactor(Factor *factor);
void releaseProgram(Program *program);

#endif
