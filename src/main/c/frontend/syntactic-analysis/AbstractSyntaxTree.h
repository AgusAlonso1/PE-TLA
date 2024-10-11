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
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;
typedef union Type Type;
typedef struct Variable Variable;
typedef struct PromiseReturn PromiseReturn;
typedef struct Declaration Declaration;
typedef struct FunctionCall FunctionCall;
typedef struct ValueList ValueList;
typedef struct VariableType VariableType;


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
	FACTOR
};

enum FactorType {
	CONSTANT,
	VARIABLE,
	FUNCTIONCALL
};

enum DeclarationType {
	LET,
	CONST, 
	VAR
};

enum StatementType {
	IF,
	WHILE,
	FOR,
	ASSIGNMENT,
	DECLARATION,
	EXPRESSION_STATEMENT
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
	DataType * unionType;
};

struct Constant {
	union value
	{
		int integer;
		float floating;
		char * string;
		char boolean;
	};
	DataType type;
};

struct VariableType {
	char * id;
	DataType type;
};

struct Variable {
	VariableType * variableType;
	Expression * value;
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
		Constant * constant;
		Variable * expression;
		FunctionCall * functionCall;
	};
	FactorType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};
	ExpressionType type;
};

struct FunctionCall {
	char * id;
	ValueList * arguments;
};

struct ValueList {
	Expression * expression;
	ValueList * next;
};

struct Program {
	Expression * expression;
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);

#endif
