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

DataType getDataType(char * name);

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant *constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression *leftExpression, Expression *rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor *factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression *expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant *constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor *factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}

Program * ExpressionProgramSemanticAction(CompilerState *compilerState, Expression *expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program *program = calloc(1, sizeof(Program));
	program->expression = expression;
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

Type * SingleTypeSemanticAction(char * singleType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type *type = malloc(sizeof(Type));
	type->singleType = malloc(sizeof(DataType));
	type->singleType = getDataType(singleType);
	return type;
}

Type * UnionTypeSemanticAction(char * firstType, ...) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	Type * type = malloc(sizeof(Type));
	unsigned int i = 0;

	type->unionType = realloc(type->unionType, (i + 1) * sizeof(DataType));
	type->unionType[i] = getDataType(firstType);

	va_list args;
	va_start(args, firstType);

	char * currentType;
	while ((currentType = va_arg(args, char *)) != NULL) {
		type->unionType = realloc(type->unionType, (i + 1) * sizeof(DataType));
		type->unionType[i] = getDataType(currentType);
	}
	return type;
}


DataType getDataType(char * name) {
	if(name == NULL) {
		return ANY;
	}
    if (strcmp(name, "number") == 0) {
        return NUMBER;
    } else if (strcmp(name, "string") == 0) {
        return STRING;
    } else if (strcmp(name, "boolean") == 0) {
        return BOOLEAN;
    } else if (strcmp(name, "undefined") == 0) {
        return UNDEFINED;
    } else if (strcmp(name, "void") == 0) {
        return VOID;
    }
	return ANY;
}

Variable * VariableSemanticAction(char * id, char * typeName) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Variable * variable = malloc(sizeof(Variable));
	variable->id = malloc(strlen(id) + 1);
	strcpy(variable->id, id);
	variable->type = getDataType(typeName);	
	variable->type = typeName;
	
	return variable;
}


Type * ReturnTypeSemanticAction(char * type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Type * returnType = malloc(sizeof(Type));
	returnType->singleType = getDataType(type);
	return returnType;
}

PromiseReturn * PromiseReturnSemanticAction(Type *returnType) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	PromiseReturn * promiseReturn = malloc(sizeof(PromiseReturn));
	promiseReturn->type = returnType;
	return promiseReturn;
}

Declaration * LetDeclarationSemanticAction() {
	
}
