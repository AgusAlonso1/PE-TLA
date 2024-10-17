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
typedef enum IncDecType IncDecType;
typedef enum IncDecPosition IncDecPosition;
typedef enum ForLoopType ForLoopType;
typedef enum IterableType IterableType;
typedef enum Assign Assign;
typedef enum UserType UserType;

typedef struct Type Type;
typedef struct TypeDeclaration TypeDeclaration;
typedef struct IterableVariable IterableVariable;

typedef struct Constant Constant;
typedef struct ArrayContent ArrayContent;

typedef struct ObjectContent ObjectContent;

typedef struct IncDec IncDec;
typedef struct Expression Expression;
typedef struct Factor Factor;

typedef struct Variable Variable;
typedef struct VariableType VariableType;
typedef struct Declaration Declaration;

typedef struct ArgumentList ArgumentList;
typedef struct VariableTypeList VariableTypeList;

typedef struct ParamsFor ParamsFor;
typedef struct ForLoop ForLoop;
typedef struct IfStatement IfStatement;
typedef struct SwitchContent SwitchContent;
typedef struct SwitchStatement SwitchStatement;
typedef struct WhileLoop WhileLoop;

typedef struct Interface Interface;
typedef struct Enum Enum;

typedef struct PromiseReturnType PromiseReturnType;

typedef struct FunctionBody FunctionBody;

typedef struct FunctionCall FunctionCall;
typedef struct FunctionDeclaration FunctionDeclaration;
typedef struct ArrowFunction ArrowFunction;
typedef struct AsyncFunction AsyncFunction;

typedef struct Statement Statement;
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
	TYPE_DECLARATION_ST,
	INC_DEC_ST,
	RETURN_ST,
	ASSIGN_ST,
	SWITCH_ST
};

enum DataType {
	NUMBER,
	STRING,
	BOOLEAN,
	ANY,
	UNDEFINED,
	VOID,
	NEVER,
	UNKNOWN,
	NBR_ARRAY,
	STR_ARRAY,
	BOOL_ARRAY,
	ANY_ARRAY,
	VOID_ARRAY,
	UNDEF_ARRAY,
	NEVER_ARRAY,
	UNK_ARRAY // unknown array
};

enum Assign {
	OBJECT_VAR,
	ARRAY_VAR,
	FUNCTIONCALL_VAR,
	ARROWFUNCTION_VAR,
	EXPRESSION_VAR,
	UNINITIALIZED
};

enum UserType {
	TYPE_OBJECT,
	TYPE_EXPRESSION,
	TYPE_ARRAY,
	TYPE_INTERFACE,
	TYPE_ENUM
};

enum IncDecType {
	INC_OP,
	DEC_OP
};

enum IncDecPosition {
	PREFIX,
	POSTFIX
};

enum IterableType {
	VARIABLE_IT,
	FUNCTIONCALL_IT,
	ARRAY_IT,
	OBJECT_IT
};

enum ForLoopType {
	FOR_CLASSIC,
	FOR_OF
};

struct Type {
	DataType singleType;
	Type *next;
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
	Type *type;
};

struct Variable {
	VariableType *variableType;
	union {
		Expression *expression;
		ArrayContent *arrayContent;
		ObjectContent *objectContent;
		FunctionCall *functionCall;
		ArrowFunction *arrowFunction;
	};
	Assign type;
};

struct PromiseReturnType {
	Type *type;
};

struct Declaration {
	DeclarationType type;
	Variable *variable;
};

struct TypeDeclaration {
	UserType type;
	char *id;
	union {
		ObjectContent *objectContent;
		Expression *expression;
		ArrayContent *arrayContent;
		VariableTypeList *interface;
		ArgumentList *enumm;
	};
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
	VariableTypeList *next;
};

struct await {
	Expression *expression;
};

struct ArrayContent {
	Expression *value;
	ArrayContent *next;
};

struct ObjectContent {
	char *key;
	Expression *value;
	ObjectContent *next;
};

struct IterableVariable {
	union {
     	char *variableName; // should be an array or object variable
     	FunctionCall *functionCall;
		ArrayContent *arrayContent;
		ObjectContent *objectContent;
	};
	IterableType type;
};

struct ParamsFor {
	ForLoopType type;
	union {
		struct {				   // For classic
			Declaration *init;	   
			Expression *condition; 
			union{
				Expression *update; 
				IncDec *updateIncDec;
			};   
		};
		struct {				  // for...of
			Declaration *value;	  
			IterableVariable *iterable; 
		};
	};
};

struct ForLoop {
	ParamsFor *params;
	Code *body;
};

struct IfStatement {
	Expression *condition;
	Code *thenBody;
	Code *elseBody;
};

struct SwitchContent {
	Expression *condition;
	Code *body;
	SwitchContent *next;
};

struct SwitchStatement {
	char *id;
	SwitchContent *content;
};

struct WhileLoop {
	Expression *condition;
	Code *body;
};

struct FunctionBody {
	Code *code;
	Expression *returnValue;
};

struct FunctionDeclaration {
	char *id;
	VariableTypeList *arguments;
	Type *returnType;
	FunctionBody *body;
};

struct ArrowFunction {
	VariableTypeList *arguments;
	Type *returnType;
	FunctionBody *body;
};

struct AsyncFunction {
	char *id;
	VariableTypeList *arguments;
	PromiseReturnType *promiseReturnType;
	FunctionBody *body;
};

struct Statement {
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
		TypeDeclaration *typeDeclaration;
		IncDec *incDec;
		Assign *assign;
		SwitchStatement *switchStatement;
	};
};

struct Code {
	Statement *statement;
	struct Code *next;
};

struct Program {
	Code *code;
};

/**
 * Node recursive destructors.
 */
// void releaseExpression(Expression *expression);
// void releaseFactor(Factor *factor);
// void releaseConstant(Constant *constant);

// void releaseVariable(Variable *variable);
// void releaseVariableType(VariableType *variableType);
// void releaseVariableName(char *variableName);
// void releaseVariableTypeList(VariableTypeList *variableTypeList);
// void releaseArgumentList(ArgumentList *argumentList);

// void releaseEnum(Enum *enumm);
// void releaseInterface(Interface *interface);

// void releaseDeclaration(Declaration *declaration);

// void releaseIf(IfStatement *ifStatement);
// void releaseForParams(ParamsFor *params);
// void releaseFor(ForLoop *forLoop);
// void releaseWhile(WhileLoop *whileLoop);

// void releaseFunctionCall(FunctionCall *functionCall);
// void releaseFunctionDeclaration(FunctionDeclaration *functionDeclaration);
// void releaseArrowFunction(ArrowFunction *arrowFunction);
// void releaseAysncFunction(AsyncFunction *asyncFunction);

// void releaseCode(Code *code);
// void releaseProgram(Program *program);

#endif
