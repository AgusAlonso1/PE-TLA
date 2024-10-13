#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

DataType getDataType(char *name);

Constant *IntConstantSemanticAction(int value);
Constant *FloatConstantSemanticAction(float value);
Constant *StringConstantSemanticAction(char *value);
Constant *BooleanConstantSemanticAction(char value);

Expression *ExpressionSemanticAction(Expression *leftExpression, Expression *rightExpression, ExpressionType type);
Expression *FactorExpressionSemanticAction(Factor *factor);
Expression *AwaitExpressionSemanticAction(Expression *expression);

IncDec *IncDecSemanticAction(Expression *expression, IncDecType type, IncDecPosition position);

Factor *ConstantFactorSemanticAction(Constant *constant);
Factor *VariableFactorSemanticAction(char *variableName);
Factor *ExpressionFactorSemanticAction(Expression *expression);

Type *SingleTypeSemanticAction(char *singleType);
Type *UnionTypeSemanticAction(char *firstType, ...);
Type *ReturnTypeSemanticAction(char *type);

VariableType *VariableTypeSemanticAction(char *id, Type *typeName);
ArgumentList *ArgumentListSemanticAction(Expression *expression, ArgumentList *next);
VariableTypeList *VariableTypeListSemanticAction(VariableType *variableType, VariableTypeList *next);
VariableList *VariableListSemanticAction(Variable *variable, VariableList *next);

Interface *InterfaceSemanticAction(char *id, VariableTypeList *variables);
Enum *EnumSemanticAction(char *id, ArgumentList *values);

PromiseReturnType *PromiseReturnTypeSemanticAction(Type *returnType);
Declaration *DeclarationSemanticAction(DeclarationType type, VariableType *variableType, Expression *expression);
IfStatement *IfSemanticAction(Expression *expression, StatementType *statement, Expression *elseExpression);
ParamsFor *ForSemanticAction(Declaration *init, Expression *condition, Expression *update, ForLoopType type);
ForLoop *ForLoopSemanticAction(ParamsFor *params, Code *code);
WhileLoop *WhileSemanticAction(Expression *condition, Code *code);

FunctionCall *functionCallSemanticAction(char *id, ArgumentList *arguments);
FunctionDeclaration *FunctionDeclarationSemanticAction(char *id, VariableType *arguments, DataType returnType, Code *body);
ArrowFunction *ArrowFunctionSemanticAction(VariableType *arguments, DataType returnType, Code *body);
AsyncFunction *AsyncFunctionSemanticAction(char *id, VariableTypeList *arguments, DataType returnType, Code *body);

Code *IfCodeSemanticAction(IfStatement *ifStatement, Code *next);
Code *ForCodeSemanticAction(ForLoop *forLoop, Code *next);
Code *DeclarationCodeSemanticAction(Declaration *declaration, Code *next);
Code *EnumCodeSemanticAction(Enum *enumm, Code *next);
Code *InterfaceCodeSemanticAction(Interface *interface, Code *next);
Code *FunctionCallCodeSemanticAction(FunctionCall *functionCall, Code *next);
Code *FunctionDeclarationCodeSemanticAction(FunctionDeclaration *functionDeclaration, Code *next);
Code *ArrowFunctionCodeSemanticAction(ArrowFunction *arrowFunction, Code *next);
Code *AsyncFunctionCodeSemanticAction(AsyncFunction *asyncFunction, Code *next);
Code *WhileCodeSemanticAction(WhileLoop *whileLoop, Code *next);
Code *ExpressionCodeSemanticAction(Expression *expression, Code *next);

Program *CodeProgramSemanticAction(CompilerState *compilerState, Code *code);

#endif
