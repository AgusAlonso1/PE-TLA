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

Constant *IntConstantSemanticAction(int value);
Constant *FloatConstantSemanticAction(float value);
Constant *StringConstantSemanticAction(char *value);
Constant *BooleanConstantSemanticAction(char value);

Expression *ExpressionSemanticAction(Expression *leftExpression, Expression *rightExpression, ExpressionType type);
IncDec *IncDecSemanticAction(Expression *expression, IncDecType type, IncDecPosition position);
Expression *FactorExpressionSemanticAction(Factor *factor);

Factor *ConstantFactorSemanticAction(Constant *constant);
Factor *VariableFactorSemanticAction(char *variableName);
Factor *ExpressionFactorSemanticAction(Expression *expression);
Factor *FunctionCallFactorSemanticAction(FunctionCall *functionCall);

Type *SingleTypeSemanticAction(char *singleType, Type *next);
Type *ArrayTypeSemanticAction(char *id);

DataType getDataType(char *name);
VariableType *VariableTypeSemanticAction(char *id, Type *type);
VariableTypeList *VariableTypeListSemanticAction(VariableType *variableType, VariableTypeList *next);

Variable *VariableExpressionSemanticAction(VariableType *variableType, Expression *expression);
Variable *VariableArraySemanticAction(VariableType *variableType, ArrayContent *arrayContent);
Variable *VariableObjectSemanticAction(VariableType *variableType, ObjectContent *objectContent);
Variable *VariableFunctionCallSemanticAction(VariableType *variableType, FunctionCall *functionCall);
Variable *VariableArrowFunctionSemanticAction(VariableType *variableType, ArrowFunction *arrowFunction);

PromiseReturnType *PromiseReturnTypeSemanticAction(Type *returnType);
ObjectContent *ObjectContentSemanticAction(char *key, Expression *expression, ObjectContent *next);
ArrayContent *ArrayContentSemanticAction(Expression *expression, ArrayContent *next);

Declaration *DeclarationSemanticAction(DeclarationType type, VariableType *variableType);
Declaration *DeclarationVariableSemanticAction(DeclarationType type, Variable *variable);

TypeDeclaration *ObjectTypeDeclarationSemanticAction(char *id, ObjectContent *objectContent);
TypeDeclaration *VariableTypeDeclarationSemanticAction(char *id, Expression *expression);
TypeDeclaration *ArrayTypeDeclarationSemanticAction(char *id, ArrayContent *arrayContent);
TypeDeclaration *EnumTypeDeclarationSemanticAction(char *id, ArgumentList *enumm);
TypeDeclaration *InterfaceTypeDeclarationSemanticAction(char *id, VariableTypeList *interface);

IterableVariable *IterableVariableNameSemanticAction(char *variableName);
IterableVariable *IterableVariableFunctionCallSemanticAction(FunctionCall *functionCall);
IterableVariable *IterableVariableArraySemanticAction(ArrayContent *arrayContent);
IterableVariable *IterableVariableObjectContentSemanticAction(ObjectContent *objectContent);

IfStatement *IfSemanticAction(Expression *expression, Code *statement, Code *elseStatement);

SwitchContent *SwitchContentSemanticAction(Expression *expression, Code *body, SwitchContent *next);
SwitchStatement *SwitchSemanticAction(char *id, SwitchContent *switchContent);

ParamsFor *ForParamsSemanticAction(Declaration *init, Expression *condition, Expression *update);
ParamsFor *ForParamsIncDecSemanticAction(Declaration *init, Expression *condition, IncDec *update);
ParamsFor *ForOfParamsSemanticAction(Declaration *value, IterableVariable *iterable);
ForLoop *ForSemanticAction(ParamsFor *params, Code *code);

WhileLoop *WhileSemanticAction(Expression *condition, Code *code);
Expression *AwaitExpressionSemanticAction(Expression *expression);

ArgumentList *ArgumentListSemanticAction(Expression *expression, ArgumentList *next);
FunctionCall *FunctionCallSemanticAction(char *id, ArgumentList *arguments, FunctionCallType type);

ReturnValue *ReturnExpressionSemanticAction(Expression *expression);
ReturnValue *ReturnArrowFunctionSemanticAction(ArrowFunction *arrowFunction);
ReturnValue *ReturnFunctionCallSemanticAction(FunctionCall *functionCall);
ReturnValue *ReturnAsyncFunctionSemanticAction(AsyncFunction *asyncFunction);

FunctionBody *FunctionBodySemanticAction(Code *code, ReturnValue *returnValue);
FunctionDeclaration *FunctionDeclarationSemanticAction(char *id, VariableTypeList *arguments, Type *returnType, FunctionBody *body);
ArrowFunction *ArrowFunctionSemanticAction(VariableTypeList *arguments, Type *returnType, FunctionBody *body);
AsyncFunction *AsyncFunctionSemanticAction(char *id, VariableTypeList *arguments, PromiseReturnType *promiseReturnType, FunctionBody *body);

Statement *IfStatementSemanticAction(IfStatement *ifStatement);
Statement *ForStatementSemanticAction(ForLoop *forLoop);
Statement *DeclarationStatementSemanticAction(Declaration *declaration);
Statement *TypeDeclarationStatementSemanticAction(TypeDeclaration *typeDeclaration);
Statement *FunctionCallStatementSemanticAction(FunctionCall *functionCall);
Statement *FunctionDeclarationStatementSemanticAction(FunctionDeclaration *functionDeclaration);
Statement *AsyncFunctionStatementSemanticAction(AsyncFunction *asyncFunction);
Statement *WhileStatementSemanticAction(WhileLoop *whileLoop);
Statement *ExpressionStatementSemanticAction(Expression *expression);
Statement *IncDecStatementSemanticAction(IncDec *incDec);
Statement *ReturnStatementSemanticAction(Expression *expression);
Statement *VariableStatementSemanticAction(Variable *variable);
Statement *SwitchStatementSemanticAction(SwitchStatement *switchStatement);

Code *CodeSemanticAction(Statement *statement, Code *next);
Program *CodeProgramSemanticAction(CompilerState *compilerState, Code *code);

#endif