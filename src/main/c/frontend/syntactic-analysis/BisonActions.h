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

Type *SingleTypeSemanticAction(char *singleType);
Type *UnionTypeSemanticAction(char *firstType, ...);
Type *ArrayTypeSemanticAction(char *id);

DataType getDataType(char *name);
VariableType *VariableTypeSemanticAction(char *id, Type *typeName);
VariableTypeList *VariableTypeListSemanticAction(VariableType *variableType, VariableTypeList *next);
VariableList *VariableListSemanticAction(Variable *variable, VariableList *next);
PromiseReturnType *PromiseReturnTypeSemanticAction(Type *returnType);
ObjectContent *ObjectContentSemanticAction(char *key, Expression *expression, ObjectContent *next);
ArrayContent *ArrayContentSemanticAction(Expression *expression, ArrayContent *next);

Declaration *DeclarationSemanticAction(DeclarationType type, VariableType *variableType, Expression *expression);
Declaration *DeclarationArraySemanticAction(DeclarationType type, VariableType *variableType, ArrayContent *arrayContent);
Declaration *DeclarationObjectSemanticAction(DeclarationType type, VariableType *variableType, ObjectContent *objectContent);

TypeDeclaration *ObjectTypeDeclarationSemanticAction(char *id, ObjectContent *objectContent);
TypeDeclaration *VariableTypeDeclarationSemanticAction(char *id, Expression *expression);
TypeDeclaration *ArrayDeclarationSemanticAction(char *id, ArrayContent *arrayContent);
TypeDeclaration *EnumTypeDeclarationSemanticAction(char *id, VariableList *enumm);
TypeDeclaration *InterfaceTypeDeclarationSemanticAction(char *id, VariableTypeList *interface);

IfStatement *IfSemanticAction(Expression *expression, StatementType *statement, Expression *elseExpression);
ParamsFor *ForSemanticAction(Declaration *init, Expression *condition, Expression *update, VariableList *iterable, ForLoopType type);
ForLoop *ForLoopSemanticAction(ParamsFor *params, Code *code);
WhileLoop *WhileSemanticAction(Expression *condition, Code *code);
Expression *AwaitExpressionSemanticAction(Expression *expression);

ArgumentList *ArgumentListSemanticAction(Expression *expression, ArgumentList *next);
FunctionCall *functionCallSemanticAction(char *id, ArgumentList *arguments);
FunctionDeclaration *FunctionDeclarationSemanticAction(char *id, VariableType *arguments, Type *returnType, Code *body);
ArrowFunction *ArrowFunctionSemanticAction(VariableType *arguments, Type *returnType, Code *body);
AsyncFunction *AsyncFunctionSemanticAction(char *id, VariableTypeList *arguments, Type *returnType, Code *body);

Code *IfCodeSemanticAction(IfStatement *ifStatement, Code *next);
Code *ForCodeSemanticAction(ForLoop *forLoop, Code *next);
Code *DeclarationCodeSemanticAction(Declaration *declaration, Code *next);
Code *TypeDeclarationCodeSemanticAction(TypeDeclaration *typeDeclaration, Code *next);
Code *FunctionCallCodeSemanticAction(FunctionCall *functionCall, Code *next);
Code *FunctionDeclarationCodeSemanticAction(FunctionDeclaration *functionDeclaration, Code *next);
Code *ArrowFunctionCodeSemanticAction(ArrowFunction *arrowFunction, Code *next);
Code *AsyncFunctionCodeSemanticAction(AsyncFunction *asyncFunction, Code *next);
Code *WhileCodeSemanticAction(WhileLoop *whileLoop, Code *next);
Code *ExpressionCodeSemanticAction(Expression *expression, Code *next);

Program *CodeProgramSemanticAction(CompilerState *compilerState, Code *code);

#endif