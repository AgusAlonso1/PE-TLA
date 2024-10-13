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

Factor *ConstantFactorSemanticAction(Constant *constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}
// Factor ---------------------------------------------------------------------------------------------------------------------
Factor *VariableFactorSemanticAction(char *variableName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->variableName = variableName;
	factor->type = VARIABLE;
	return factor;
}
// Expression -------------------------------------------------------------------------------------------------------------------
Factor *ExpressionFactorSemanticAction(Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = malloc(sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}
// Type -------------------------------------------------------------------------------------------------------------------------
Type *SingleTypeSemanticAction(char *singleType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type *type = malloc(sizeof(Type));
	type->singleType = malloc(sizeof(DataType));
	type->singleType = getDataType(singleType);
	return type;
}

Type *UnionTypeSemanticAction(char *firstType, ...) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	Type *type = malloc(sizeof(Type));
	unsigned int i = 0;

	type->unionType = realloc(type->unionType, (i + 1) * sizeof(DataType));
	type->unionType[i] = getDataType(firstType);

	va_list args;
	va_start(args, firstType);

	char *currentType;
	while ((currentType = va_arg(args, char *)) != NULL) {
		type->unionType = realloc(type->unionType, (i + 1) * sizeof(DataType));
		type->unionType[i] = getDataType(currentType);
	}
	return type;
}

Type *ArrayTypeSemanticAction(char *id) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type *type = malloc(sizeof(Type));
	// terminar
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
VariableType *VariableTypeSemanticAction(char *id, Type *typeName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableType *variableType = malloc(sizeof(VariableType));
	variableType->id = malloc(strlen(id) + 1);
	strcpy(variableType->id, id);
	variableType->type = getDataType(typeName);
	variableType->type = typeName;
	return variableType;
}

VariableTypeList *VariableTypeListSemanticAction(VariableType *variableType, VariableTypeList *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableTypeList *variableTypeList = malloc(sizeof(VariableTypeList));
	variableTypeList->variableType = variableType;
	variableTypeList->next = next;
	return variableTypeList;
}

VariableList *VariableListSemanticAction(Variable *variable, VariableList *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VariableList *variableList = malloc(sizeof(VariableList));
	variableList->variable = variable;
	variableList->next = next;
	return variableList;
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

// Declaration -------------------------------------------------------------------------------------------------------------------------
Declaration *DeclarationSemanticAction(DeclarationType type, VariableType *variableType, Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration *declaration = malloc(sizeof(Declaration));
	declaration->variable = malloc(sizeof(Variable));
	declaration->variable->variableType = variableType;
	declaration->variable->expression = expression;
	declaration->variable->type = TYPE_EXPRESSION;
	declaration->type = type;
	return declaration;
}

Declaration *DeclarationArraySemanticAction(DeclarationType type, VariableType *variableType, ArrayContent *arrayContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration *declaration = malloc(sizeof(Declaration));
	declaration->type = type;
	declaration->variable = malloc(sizeof(Variable));
	declaration->variable->variableType = variableType;
	declaration->variable->arrayContent = arrayContent;
	declaration->variable->type = TYPE_ARRAY;
	return declaration;
}

Declaration *DeclarationObjectSemanticAction(DeclarationType type, VariableType *variableType, ObjectContent *objectContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Declaration *declaration = malloc(sizeof(Declaration));
	declaration->type = type;
	declaration->variable = malloc(sizeof(Variable));
	declaration->variable->variableType = variableType;
	declaration->variable->objectContent = objectContent;
	declaration->variable->type = TYPE_OBJECT;
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

TypeDeclaration *ArrayDeclarationSemanticAction(char *id, ArrayContent *arrayContent) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TypeDeclaration *typeDeclaration = malloc(sizeof(TypeDeclaration));
	typeDeclaration->id = malloc(strlen(id) + 1);
	strcpy(typeDeclaration->id, id);
	typeDeclaration->arrayContent = arrayContent;
	typeDeclaration->type = TYPE_ARRAY;
	return typeDeclaration;
}

TypeDeclaration *EnumTypeDeclarationSemanticAction(char *id, VariableList *enumm) {
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
IfStatement *IfSemanticAction(Expression *expression, StatementType *statement, Expression *elseExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IfStatement *ifStatement = malloc(sizeof(IfStatement));
	ifStatement->condition = expression;
	ifStatement->thenBody = statement;
	ifStatement->elseBody = elseExpression;
	return ifStatement;
}
// VER
ParamsFor *ForSemanticAction(Declaration *init, Expression *condition, Expression *update, VariableList *iterable, ForLoopType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ParamsFor *paramsFor = malloc(sizeof(ParamsFor));
	paramsFor->type = type;
	switch (type) {
		case FOR_CLASSIC:
			paramsFor->forClassic.init = init;
			paramsFor->forClassic.condition = condition;
			paramsFor->forClassic.update = update;
			break;
		case FOR_OF:
			paramsFor->forOf.value = init;
			paramsFor->forOf.iterable = iterable;
			break;
	}
	return paramsFor;
}

ForLoop *ForLoopSemanticAction(ParamsFor *params, Code *code) {
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
	ArgumentList *argList = malloc(sizeof(ArgumentList));
	argList->expression = malloc(strlen(expression) + 1);
	strcpy(argList->expression, expression);
	argList->next = next;
	return argList;
}

FunctionCall *functionCallSemanticAction(char *id, ArgumentList *arguments) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FunctionCall *functionCall = malloc(sizeof(FunctionCall));
	functionCall->id = malloc(strlen(id) + 1);
	strcpy(functionCall->id, id);
	functionCall->arguments = arguments;
	return functionCall;
}

FunctionDeclaration *FunctionDeclarationSemanticAction(char *id, VariableType *arguments, Type *returnType, Code *body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FunctionDeclaration *functionDeclaration = malloc(sizeof(FunctionDeclaration));
	functionDeclaration->id = malloc(strlen(id) + 1);
	strcpy(functionDeclaration->id, id);
	functionDeclaration->arguments = arguments;
	functionDeclaration->returnType = returnType;
	functionDeclaration->body = body;
	return functionDeclaration;
}

ArrowFunction *ArrowFunctionSemanticAction(VariableType *arguments, Type *returnType, Code *body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArrowFunction *arrowFunction = malloc(sizeof(ArrowFunction));
	arrowFunction->arguments = arguments;
	arrowFunction->returnType = returnType;
	arrowFunction->body = body;
	return arrowFunction;
}

AsyncFunction *AsyncFunctionSemanticAction(char *id, VariableTypeList *arguments, Type *returnType, Code *body) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	AsyncFunction *asyncFunction = malloc(sizeof(asyncFunction));
	asyncFunction->id = malloc(strlen(id) + 1);
	strcpy(asyncFunction->id, id);
	asyncFunction->arguments = arguments;
	asyncFunction->returnType = returnType;
	asyncFunction->body = body;
	return asyncFunction;
}

// Code -------------------------------------------------------------------------------------------------------------------------
Code *IfCodeSemanticAction(IfStatement *ifStatement, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = IF_ST;
	code->ifStatement = ifStatement;
	code->next = next;
	return code;
}

Code *ForCodeSemanticAction(ForLoop *forLoop, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = FOR_ST;
	code->forLoop = forLoop;
	code->next = next;
	return code;
}

Code *DeclarationCodeSemanticAction(Declaration *declaration, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = DECLARATION_ST;
	code->declaration = declaration;
	code->next = next;
	return code;
}

Code *TypeDeclarationCodeSemanticAction(TypeDeclaration *typeDeclaration, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = TYPE_DECLARATION_ST;
	code->typeDeclaration = typeDeclaration;
	code->next = next;
	return code;
}

Code *FunctionCallCodeSemanticAction(FunctionCall *functionCall, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = FUNCTIONCALL_ST;
	code->functionCall = functionCall;
	code->next = next;
	return code;
}

Code *FunctionDeclarationCodeSemanticAction(FunctionDeclaration *functionDeclaration, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = FUNCTION_DECLARATION_ST;
	code->FunctionDeclaration = functionDeclaration;
	code->next = next;
	return code;
}

Code *ArrowFunctionCodeSemanticAction(ArrowFunction *arrowFunction, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = ARROW_FUNCTION_ST;
	code->arrowFunction = arrowFunction;
	code->next = next;
	return code;
}

Code *AsyncFunctionCodeSemanticAction(AsyncFunction *asyncFunction, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = ASYNC_FUNCTION_ST;
	code->asyncFunction = asyncFunction;
	code->next = next;
	return code;
}

Code *WhileCodeSemanticAction(WhileLoop *whileLoop, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = WHILE_ST;
	code->whileLoop = whileLoop;
	code->next = next;
	return code;
}

Code *ExpressionCodeSemanticAction(Expression *expression, Code *next) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Code *code = malloc(sizeof(Code));
	code->statement = EXPRESSION_ST;
	code->expression = expression;
	code->next = next;
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
