#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char *functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char *functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */
// Constants -------------------------------------------------------------------------------------------------------------------------
Constant *IntConstantSemanticAction(int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant *constant = malloc(sizeof(Constant));
	constant->integer = value;
	constant->type = NUMBER;
	return constant;
}

Constant *FloatConstantSemanticAction(float value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant *constant = malloc(sizeof(Constant));
	constant->floating = value;
	constant->type = NUMBER;
	return constant;
}

Constant *StringConstantSemanticAction(char *value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant *constant = malloc(sizeof(Constant));
	constant->string = malloc(strlen(value) + 1);
	strcpy(constant->string, value);
	constant->type = STRING;
	return constant;
}

Constant *BooleanConstantSemanticAction(char value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant *constant = malloc(sizeof(Constant));
	constant->boolean = value;
	constant->type = BOOLEAN;
	return constant;
}
// Expression -------------------------------------------------------------------------------------------------------------------
Expression *ExpressionSemanticAction(Expression *leftExpression, Expression *rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *expression = malloc(sizeof(Expression));
	expression->type = type;
	switch (type) {
		case NOT_OP:
			expression->expression = leftExpression;
			break;
		default:
			expression->leftExpression = leftExpression;
			expression->rightExpression = rightExpression;
			break;
	}
	return expression;
}

IncDec *IncDecSemanticAction(Expression *expression, IncDecType type, IncDecPosition position) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IncDec *incDecExpression = malloc(sizeof(IncDec));
	incDecExpression->expression = expression;
	incDecExpression->type = type;
	return incDecExpression;
}

Expression *FactorExpressionSemanticAction(Factor *factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *expression = malloc(sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

// Factor ---------------------------------------------------------------------------------------------------------------------
Factor *ConstantFactorSemanticAction(Constant *constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}
Factor *VariableFactorSemanticAction(char *variableName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->variableName = variableName;
	factor->type = VARIABLE;
	return factor;
}
Factor *ExpressionFactorSemanticAction(Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}
Factor *FunctionCallFactorSemanticAction(FunctionCall *functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->functionCall = functionCall;
	factor->type = FUNCTIONCALL;
	return factor;
}
// Type -------------------------------------------------------------------------------------------------------------------------
Type *SingleTypeSemanticAction(char *singleType, Type *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type *type = malloc(sizeof(Type));
	type->singleType = getDataType(singleType);
	type->next = next;
	return type;
}

Type *ArrayTypeSemanticAction(char *id) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type *type = malloc(sizeof(Type));
	type->singleType = getDataType(id);
	type->next = NULL;
	return type;
}

DataType getDataType(char *name) {
	if (name == NULL) {
		return ANY;
	}
	if (strcmp(name, "number") == 0) {
		return NUMBER;
	}
	else if (strcmp(name, "string") == 0) {
		return STRING;
	}
	else if (strcmp(name, "boolean") == 0) {
		return BOOLEAN;
	}
	else if (strcmp(name, "undefined") == 0) {
		return UNDEFINED;
	}
	else if (strcmp(name, "void") == 0) {
		return VOID;
	}
	else if (strcmp(name, "never") == 0) {
		return NEVER;
	}
	else if (strcmp(name, "unknown") == 0) {
		return UNKNOWN;
	}
	else if (strcmp(name, "number[]") == 0) {
		return NBR_ARRAY;
	}
	else if (strcmp(name, "string[]") == 0) {
		return STR_ARRAY;
	}
	else if (strcmp(name, "boolean[]") == 0) {
		return BOOL_ARRAY;
	}
	else if (strcmp(name, "any[]") == 0) {
		return ANY_ARRAY;
	}
	else if (strcmp(name, "void[]") == 0) {
		return VOID_ARRAY;
	}
	else if (strcmp(name, "undefined[]") == 0) {
		return UNDEF_ARRAY;
	}
	else if (strcmp(name, "never[]") == 0) {
		return NEVER_ARRAY;
	}
	else if (strcmp(name, "unknown[]") == 0) {
		return UNK_ARRAY;
	}
	return ANY;
}

// Variable -------------------------------------------------------------------------------------------------------------------------
VariableType *VariableTypeSemanticAction(char *id, Type *type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableType *variableType = malloc(sizeof(VariableType));
	variableType->id = malloc(strlen(id) + 1);
	strcpy(variableType->id, id);
	variableType->type = type;
	return variableType;
}

VariableTypeList *VariableTypeListSemanticAction(VariableType *variableType, VariableTypeList *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableTypeList *variableTypeList = malloc(sizeof(VariableTypeList));
	variableTypeList->variableType = variableType;
	variableTypeList->next = next;
	return variableTypeList;
}

Variable *VariableSemanticAction(VariableType *variableType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable *assignVariable = malloc(sizeof(Variable));
	assignVariable->variableType = variableType;
	assignVariable->expression = NULL;
	assignVariable->type = UNINITIALIZED;
	return assignVariable;
}

Variable *VariableExpressionSemanticAction(VariableType *variableType, Expression *expression){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable *assignVariable = malloc(sizeof(Variable));
	assignVariable->variableType = variableType;
	assignVariable->expression = expression;
	assignVariable->type = EXPRESSION_VAR;
	return assignVariable;
}

Variable *VariableArraySemanticAction(VariableType *variableType, ArrayContent *arrayContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable *assignVariable = malloc(sizeof(Variable));
	assignVariable->variableType = variableType;
	assignVariable->arrayContent = arrayContent;
	assignVariable->type = ARRAY_VAR;
	return assignVariable;
}

Variable *VariableObjectSemanticAction(VariableType *variableType, ObjectContent *objectContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable *assignVariable = malloc(sizeof(Variable));
	assignVariable->variableType = variableType;
	assignVariable->objectContent = objectContent;
	assignVariable->type = OBJECT_VAR;
	return assignVariable;
}

Variable *VariableFunctionCallSemanticAction(VariableType *variableType, FunctionCall *functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable *assignVariable = malloc(sizeof(Variable));
	assignVariable->variableType = variableType;
	assignVariable->functionCall = functionCall;
	assignVariable->type = FUNCTIONCALL_VAR;
	return assignVariable;
}

Variable *VariableArrowFunctionSemanticAction(VariableType *variableType, ArrowFunction *arrowFunction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable *assignVariable = malloc(sizeof(Variable));
	assignVariable->variableType = variableType;
	assignVariable->arrowFunction = arrowFunction;
	assignVariable->type = ARROWFUNCTION_VAR;
	return assignVariable;
}

// PromiseReturnType -------------------------------------------------------------------------------------------------------------------------
PromiseReturnType *PromiseReturnTypeSemanticAction(Type *returnType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PromiseReturnType *promiseReturn = malloc(sizeof(PromiseReturnType));
	promiseReturn->type = returnType;
	return promiseReturn;
}

// Content -------------------------------------------------------------------------------------------------------------------------
ObjectContent *ObjectContentSemanticAction(char *key, Expression *expression, ObjectContent *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ObjectContent *objectContent = malloc(sizeof(ObjectContent));
	objectContent->key = malloc(strlen(key) + 1);
	strcpy(objectContent->key, key);
	objectContent->value = expression;
	objectContent->next = next;
	return objectContent;
}

ArrayContent *ArrayContentSemanticAction(Expression *expression, ArrayContent *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArrayContent *arrayContent = malloc(sizeof(ArrayContent));
	arrayContent->value = expression;
	arrayContent->next = next;
	return arrayContent;
}
// IterableVariable -------------------------------------------------------------------------------------------------------------------------
IterableVariable *IterableVariableNameSemanticAction(char *variableName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IterableVariable *iterableVariable = malloc(sizeof(IterableVariable));
	iterableVariable->variableName = variableName;
	iterableVariable->type = VARIABLE_IT;
	return iterableVariable;
}

IterableVariable *IterableVariableFunctionCallSemanticAction(FunctionCall *functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IterableVariable *iterableVariable = malloc(sizeof(IterableVariable));
	iterableVariable->functionCall = functionCall;
	iterableVariable->type = FUNCTIONCALL_IT;
	return iterableVariable;
}

IterableVariable *IterableVariableArraySemanticAction(ArrayContent *arrayContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IterableVariable *iterableVariable = malloc(sizeof(IterableVariable));
	iterableVariable->arrayContent = arrayContent;
	iterableVariable->type = ARRAY_IT;
	return iterableVariable;
}

IterableVariable *IterableVariableObjectContentSemanticAction(ObjectContent *objectContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IterableVariable *iterableVariable = malloc(sizeof(IterableVariable));
	iterableVariable->objectContent = objectContent;
	iterableVariable->type = OBJECT_IT;
	return iterableVariable;
}


// Declaration -------------------------------------------------------------------------------------------------------------------------
Declaration *DeclarationSemanticAction(DeclarationType type, VariableType *variableType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration *declaration = malloc(sizeof(Declaration));
	declaration->variable = malloc(sizeof(Variable));
	declaration->variable->variableType = variableType;
	declaration->variable->type = UNINITIALIZED;
	declaration->type = type;
	return declaration;
}

Declaration *DeclarationVariableSemanticAction(DeclarationType type, Variable *variable) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration *declaration = malloc(sizeof(Declaration));
	declaration->variable = malloc(sizeof(Variable));
	declaration->variable = variable;
	declaration->type = type;
	return declaration;
}

// TypeDeclaration -------------------------------------------------------------------------------------------------------------------------
TypeDeclaration *ObjectTypeDeclarationSemanticAction(char *id, ObjectContent *objectContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TypeDeclaration *typeDeclaration = malloc(sizeof(TypeDeclaration));
	typeDeclaration->id = malloc(strlen(id) + 1);
	strcpy(typeDeclaration->id, id);
	typeDeclaration->objectContent = objectContent;
	typeDeclaration->type = TYPE_OBJECT;
	return typeDeclaration;
}

TypeDeclaration *VariableTypeDeclarationSemanticAction(char *id, Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TypeDeclaration *typeDeclaration = malloc(sizeof(TypeDeclaration));
	strcpy(typeDeclaration->id, id);
	typeDeclaration->expression = expression;
	typeDeclaration->type = TYPE_EXPRESSION;
	return typeDeclaration;
}

TypeDeclaration *ArrayTypeDeclarationSemanticAction(char *id, ArrayContent *arrayContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TypeDeclaration *typeDeclaration = malloc(sizeof(TypeDeclaration));
	typeDeclaration->id = malloc(strlen(id) + 1);
	strcpy(typeDeclaration->id, id);
	typeDeclaration->arrayContent = arrayContent;
	typeDeclaration->type = TYPE_ARRAY;
	return typeDeclaration;
}

TypeDeclaration *EnumTypeDeclarationSemanticAction(char *id, ArgumentList *enumm) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TypeDeclaration *typeDeclaration = malloc(sizeof(TypeDeclaration));
	typeDeclaration->id = malloc(strlen(id) + 1);
	strcpy(typeDeclaration->id, id);
	typeDeclaration->enumm = enumm;
	typeDeclaration->type = TYPE_ENUM;
	return typeDeclaration;
}

TypeDeclaration *InterfaceTypeDeclarationSemanticAction(char *id, VariableTypeList *interface) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TypeDeclaration *typeDeclaration = malloc(sizeof(TypeDeclaration));
	typeDeclaration->id = malloc(strlen(id) + 1);
	strcpy(typeDeclaration->id, id);
	typeDeclaration->interface = interface;
	typeDeclaration->type = TYPE_INTERFACE;
	return typeDeclaration;
}

// FLow Control -------------------------------------------------------------------------------------------------------------------------
IfStatement *IfSemanticAction(Expression *expression, Code *statement, Code *elseStatement) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IfStatement *ifStatement = malloc(sizeof(IfStatement));
	ifStatement->condition = expression;
	ifStatement->thenBody = statement;
	ifStatement->elseBody = elseStatement;
	return ifStatement;
}

SwitchContent *SwitchContentSemanticAction(Expression *expression, Code *body, SwitchContent *next){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SwitchContent *switchContent = malloc(sizeof(SwitchContent));
	switchContent->condition = expression;
	switchContent->body = body;
	switchContent->next = next;
	return switchContent;
}

SwitchStatement *SwitchSemanticAction(char *id, SwitchContent *switchContent){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SwitchStatement *switchStatement = malloc(sizeof(SwitchStatement));
	switchStatement->id = id;
	switchStatement->content = switchContent;
	return switchStatement;
}

ParamsFor *ForParamsSemanticAction(Declaration *init, Expression *condition, Expression *update) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ParamsFor *params = malloc(sizeof(ParamsFor));
	params->init = init;
	params->condition = condition;
	params->update = update;
	params->type = FOR_CLASSIC;
	return params;
}

ParamsFor *ForParamsIncDecSemanticAction(Declaration *init, Expression *condition, IncDec *update) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ParamsFor *params = malloc(sizeof(ParamsFor));
	params->init = init;
	params->condition = condition;
	params->updateIncDec = update;
	params->type = FOR_CLASSIC;
	return params;
}

ParamsFor *ForOfParamsSemanticAction(Declaration *value, IterableVariable *iterable) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ParamsFor *params = malloc(sizeof(ParamsFor));
	params->value = value;
	params->iterable = iterable;
	params->type = FOR_OF;
	return params;
}

ForLoop *ForSemanticAction(ParamsFor *params, Code *code) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ForLoop *forLoop = malloc(sizeof(ForLoop));
	forLoop->params = params;
	forLoop->body = code;
	return forLoop;
}

WhileLoop *WhileSemanticAction(Expression *condition, Code *code) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WhileLoop *whileLoop = malloc(sizeof(WhileLoop));
	whileLoop->condition = condition;
	whileLoop->body = code;
	return whileLoop;
}
// Await -------------------------------------------------------------------------------------------------------------------------
Expression *AwaitExpressionSemanticAction(Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *newExpression = (Expression *) malloc(sizeof(Expression));
	newExpression->type = AWAIT_OP;
	newExpression->expression = expression;
	return newExpression;
}

// Functions -------------------------------------------------------------------------------------------------------------------------
ArgumentList *ArgumentListSemanticAction(Expression *expression, ArgumentList *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (expression == NULL) {
		return NULL;
	}
	ArgumentList *argList = malloc(sizeof(ArgumentList));
	argList->expression = malloc(sizeof(Expression));
	argList->expression = expression;
	argList->next = next;
	return argList;
}

FunctionCall *FunctionCallSemanticAction(char *id, ArgumentList *arguments) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (id == NULL) {
		return NULL;
	}
	FunctionCall *functionCall = malloc(sizeof(FunctionCall));
	functionCall->id = malloc(strlen(id) + 1);
	strcpy(functionCall->id, id);
	functionCall->arguments = arguments;
	return functionCall;
}

FunctionBody *FunctionBodySemanticAction(Code *code, Expression *returnValue) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (code == NULL && returnValue == NULL) {
		return NULL;
	}
	FunctionBody *functionBody = malloc(sizeof(FunctionBody));
	functionBody->code = code;
	functionBody->returnValue = returnValue;
	return functionBody;
}

FunctionDeclaration *FunctionDeclarationSemanticAction(char *id, VariableTypeList *arguments, Type *returnType, FunctionBody *body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (id == NULL) {
		return NULL;
	}
	FunctionDeclaration *functionDeclaration = malloc(sizeof(FunctionDeclaration));
	functionDeclaration->id = malloc(strlen(id) + 1);
	strcpy(functionDeclaration->id, id);
	functionDeclaration->arguments = arguments;
	functionDeclaration->returnType = returnType;
	functionDeclaration->body = body;
	return functionDeclaration;
}

ArrowFunction *ArrowFunctionSemanticAction(VariableTypeList *arguments, Type *returnType, FunctionBody *body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (arguments == NULL) {
		return NULL;
	}
	ArrowFunction *arrowFunction = malloc(sizeof(ArrowFunction));
	arrowFunction->arguments = arguments;
	arrowFunction->returnType = returnType;
	arrowFunction->body = body;
	return arrowFunction;
}

AsyncFunction *AsyncFunctionSemanticAction(char *id, VariableTypeList *arguments, PromiseReturnType *promiseReturnType, FunctionBody *body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (id == NULL) {
		return NULL;
	}
	AsyncFunction *asyncFunction = malloc(sizeof(asyncFunction));
	asyncFunction->id = malloc(strlen(id) + 1);
	strcpy(asyncFunction->id, id);
	asyncFunction->arguments = arguments;
	asyncFunction->promiseReturnType = promiseReturnType;
	asyncFunction->body = body;
	return asyncFunction;
}

// Code -------------------------------------------------------------------------------------------------------------------------
Statement *IfStatementSemanticAction(IfStatement *ifStatement) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (ifStatement == NULL) {
		return NULL;
	}
	Statement *code = malloc(sizeof(Code));
	code->statement = IF_ST;
	code->ifStatement = ifStatement;
	return code;
}

Statement *ForStatementSemanticAction(ForLoop *forLoop) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (forLoop == NULL) {
		return NULL;
	}
	Statement *code = malloc(sizeof(Code));
	code->statement = FOR_ST;
	code->forLoop = forLoop;
	return code;
}

Statement *DeclarationStatementSemanticAction(Declaration *declaration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (declaration == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = DECLARATION_ST;
	stat->declaration = declaration;
	return stat;
}

Statement *TypeDeclarationStatementSemanticAction(TypeDeclaration *typeDeclaration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (typeDeclaration == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = TYPE_DECLARATION_ST;
	stat->typeDeclaration = typeDeclaration;
	return stat;
}

Statement *FunctionCallStatementSemanticAction(FunctionCall *functionCall) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (functionCall == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = FUNCTIONCALL_ST;
	stat->functionCall = functionCall;
	return stat;
}

Statement *FunctionDeclarationStatementSemanticAction(FunctionDeclaration *functionDeclaration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (functionDeclaration == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = FUNCTION_DECLARATION_ST;
	stat->FunctionDeclaration = functionDeclaration;
	return stat;
}

Statement *ArrowFunctionStatementSemanticAction(ArrowFunction *arrowFunction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (arrowFunction == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = ARROW_FUNCTION_ST;
	stat->arrowFunction = arrowFunction;
	return stat;
}

Statement *AsyncFunctionStatementSemanticAction(AsyncFunction *asyncFunction) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (asyncFunction == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = ASYNC_FUNCTION_ST;
	stat->asyncFunction = asyncFunction;
	return stat;
}

Statement *WhileStatementSemanticAction(WhileLoop *whileLoop) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (whileLoop == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = WHILE_ST;
	stat->whileLoop = whileLoop;
	return stat;
}

Statement *ExpressionStatementSemanticAction(Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (expression == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = EXPRESSION_ST;
	stat->expression = expression;
	return stat;
}

Statement *IncDecStatementSemanticAction(IncDec *incDec) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (incDec == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Code));
	stat->statement = INC_DEC_ST;
	stat->incDec = incDec;
	return stat;
}

Statement *ReturnStatementSemanticAction(Expression *returnValue) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (returnValue == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Statement));
	stat->statement = RETURN_ST;
	return stat;
}

Statement *VariableStatementSemanticAction(Variable *variable) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (variable == NULL) {
		return NULL;
	}
	Statement *stat = malloc(sizeof(Statement));
	stat->statement = VARIABLE_ST;
	stat->variable = variable;
	return stat;
}

Statement *SwitchStatementSemanticAction(SwitchStatement *switchStatement) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (switchStatement == NULL) {
		return NULL;
	}
	Statement *code = malloc(sizeof(Code));
	code->statement = SWITCH_ST;
	code->switchStatement = switchStatement;
	return code;
}

// Code -------------------------------------------------------------------------------------------------------------------------
Code *CodeSemanticAction(Statement *statement, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	if (statement == NULL) {
		return NULL;
	}
	Code *code = malloc(sizeof(Code));
	code->statement = statement;
	return code;
}

// Program -------------------------------------------------------------------------------------------------------------------------
Program *CodeProgramSemanticAction(CompilerState *compilerState, Code *code) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program *program = calloc(1, sizeof(Program));
	program->code = code;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}
