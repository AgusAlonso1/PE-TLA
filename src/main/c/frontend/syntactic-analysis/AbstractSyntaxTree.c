#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseExpression(Expression *expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case NOT_OP:
				releaseExpression(expression->expression);
				break;
			case FACTOR:
				releaseFactor(expression->factor);
				break;
			default:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor *factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case VARIABLE:
				releaseVariableName(factor->variableName);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
			case FUNCTIONCALL:
				releaseFunctionCall(factor->functionCall);
				break;
		}
		free(factor);
	}
}

void releaseConstant(Constant *constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		switch (constant->type) {
			case STRING:
				free(constant->string);
				break;
		}
		free(constant);
	}
}

void releaseVariable(Variable *variable) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (variable != NULL) {
		releaseVariableType(variable->variableType);
		releaseExpression(variable->value);
		free(variable);
	}
}

void releaseVariableType(VariableType *variableType) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (variableType != NULL) {
		free(variableType->id);
		free(variableType);
	}
}

void releaseVariableName(char *variableName) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	free(variableName);
}

void releaseFunctionCall(FunctionCall *functionCall) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (functionCall != NULL) {
		free(functionCall->id);
		releaseValueList(functionCall->arguments);
	}
}

void releaseVariableTypeList(VariableTypeList *variableTypeList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (variableTypeList != NULL) {
		releaseVariableTypeList(variableTypeList->next);
		releaseVariableType(variableTypeList->variable);
		free(variableTypeList);
	}
}

void releaseValueList(ValueList *valueList) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (valueList != NULL) {
		releaseValueList(valueList->next);
	}
	releaseExpression(valueList->expression);
	free(valueList);
}

void releaseIf(IfStatement *ifStatement) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (ifStatement != NULL) {
		releaseExpression(ifStatement->condition);
		releaseCode(ifStatement->thenBody);
		releaseCode(ifStatement->elseBody);
		free(ifStatement);
	}
}

void releaseForParams(ParamsFor *params) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (params != NULL) {
		switch (params->type) {
			case FOR_CLASSIC:
				releaseDeclaration(params->forClassic.init);
				releaseExpression(params->forClassic.condition);
				releaseExpression(params->forClassic.update);
				break;
			default:
				releaseDeclaration(params->forOf.value);
				releaseExpression(params->forOf.iterable);
				break;
		}
		free(params);
	}
}

void releaseFor(ForLoop *forLoop) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (forLoop != NULL) {
		releaseForParams(forLoop->params);
		releaseCode(forLoop->body);
		free(forLoop);
	}
}

void releaseCode(Code *code) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (code != NULL) {
		switch (code->statement) {
			case IF:
				releaseIf(code->ifStatement);
				break;
			case FOR:
				releaseFor(code->forLoop);
				break;
			case WHILE:
				releaseWhile(code->whileLoop);
				break;
			case FUNCTION_DECLARATION:
				releaseFunctionDeclaration(code->FunctionDeclaration);
				break;
			case ARROW_FUNCTION:
				releaseArrowFunction(code->arrowFunction);
				break;
			case ASYNC_FUNCTION:
				releaseAysncFunction(code->asyncFunction);
				break;
			case DECLARATION:
				releaseDeclaration(code->declaration);
				break;
			case EXPRESSION:
				releaseExpression(code->expression);
				break;
			case VARIABLE:
				releaseVariable(code->variable);
				break;
			case FUNCTIONCALL:
				releaseFunctionCall(code->functionCall);
				break;
		}
		free(code);
	}
}

void releaseWhile(WhileLoop *whileLoop) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (whileLoop != NULL) {
		releaseExpression(whileLoop->condition);
		releaseCode(whileLoop->body);
		free(whileLoop);
	}
}

void releaseFunctionDeclaration(FunctionDeclaration *functionDeclaration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (functionDeclaration != NULL) {
		free(functionDeclaration->id);
		releaseVariableTypeList(functionDeclaration->arguments);
		releaseCode(functionDeclaration->body);
		free(functionDeclaration);
	}
}

void releaseArrowFunction(ArrowFunction *arrowFunction) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (arrowFunction != NULL) {
		releaseVariableTypeList(arrowFunction->arguments);
		releaseCode(arrowFunction->body);
		free(arrowFunction);
	}
}

void releaseDeclaration(Declaration *declaration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (declaration != NULL) {
		releaseVariable(declaration->variable);
		free(declaration);
	}
}

void releaseAysncFunction(AsyncFunction *asyncFunction) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (asyncFunction != NULL) {
		releaseVariableTypeList(asyncFunction->arguments);
		releaseCode(asyncFunction->body);
		free(asyncFunction);
	}
}

void releaseProgram(Program *program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseCode(program->code);
		free(program);
	}
}
