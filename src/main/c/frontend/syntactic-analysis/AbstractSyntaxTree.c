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
// Expressions -------------------------------------------------------------------------------------------
// void releaseExpression(Expression *expression) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (expression != NULL) {
// 		switch (expression->type) {
// 			case NOT_OP:
// 				releaseExpression(expression->expression);
// 				break;
// 			case FACTOR:
// 				releaseFactor(expression->factor);
// 				break;
// 			default:
// 				releaseExpression(expression->leftExpression);
// 				releaseExpression(expression->rightExpression);
// 				break;
// 		}
// 		free(expression);
// 	}
// }
// // Factors -------------------------------------------------------------------------------------------
// void releaseFactor(Factor *factor) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (factor != NULL) {
// 		switch (factor->type) {
// 			case CONSTANT:
// 				releaseConstant(factor->constant);
// 				break;
// 			case VARIABLE:
// 				releaseVariableName(factor->variableName);
// 				break;
// 			case EXPRESSION:
// 				releaseExpression(factor->expression);
// 				break;
// 			case FUNCTIONCALL:
// 				releaseFunctionCall(factor->functionCall);
// 				break;
// 		}
// 		free(factor);
// 	}
// }
// // Constants -------------------------------------------------------------------------------------------
// void releaseConstant(Constant *constant) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (constant != NULL) {
// 		switch (constant->type) {
// 			case STRING:
// 				free(constant->string);
// 				break;
// 		}
// 		free(constant);
// 	}
// }
// // Variables -------------------------------------------------------------------------------------------
// void releaseVariable(Variable *variable) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (variable != NULL) {
// 		switch (variable->type) {
// 			case TYPE_ARRAY:
// 				releaseArrayContent(variable->arrayContent);
// 				break;
// 			case TYPE_OBJECT:
// 				releaseObjectContent(variable->objectContent);
// 				break;
// 			case TYPE_EXPRESSION:
// 				releaseExpression(variable->expression);
// 				break;
// 		}
// 		releaseVariableType(variable->variableType);
// 		free(variable);
// 	}
// }

// void releaseVariableType(VariableType *variableType) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (variableType != NULL) {
// 		free(variableType->id);
// 		free(variableType);
// 	}
// }

// void releaseVariableName(char *variableName) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	free(variableName);
// }

// void releaseVariableTypeList(VariableTypeList *variableTypeList) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (variableTypeList != NULL) {
// 		releaseVariableTypeList(variableTypeList->next);
// 		releaseVariableType(variableTypeList->variableType);
// 		free(variableTypeList);
// 	}
// }

// void releaseVariableList(VariableList *variableList) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (variableList != NULL) {
// 		releaseVariableList(variableList->next);
// 		releaseVariable(variableList->variable);
// 		free(variableList);
// 	}
// }

// void releaseArgumentList(ArgumentList *argumentList) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (argumentList != NULL) {
// 		releaseArgumentList(argumentList->next);
// 	}
// 	releaseExpression(argumentList->expression);
// 	free(argumentList);
// }

// // Declarations -------------------------------------------------------------------------------------------
// void releaseDeclaration(Declaration *declaration) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (declaration != NULL) {
// 		releaseVariable(declaration->variable);
// 		free(declaration);
// 	}
// }

// // Flow control ------------------------------------------------------------------------------------------
// void releaseIf(IfStatement *ifStatement) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (ifStatement != NULL) {
// 		releaseExpression(ifStatement->condition);
// 		releaseCode(ifStatement->thenBody);
// 		releaseCode(ifStatement->elseBody);
// 		free(ifStatement);
// 	}
// }

// void releaseForParams(ParamsFor *params) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (params != NULL) {
// 		switch (params->type) {
// 			case FOR_CLASSIC:
// 				releaseDeclaration(params->forClassic.init);
// 				releaseExpression(params->forClassic.condition);
// 				releaseExpression(params->forClassic.update);
// 				break;
// 			default:
// 				releaseDeclaration(params->forOf.value);
// 				releaseExpression(params->forOf.iterable);
// 				break;
// 		}
// 		free(params);
// 	}
// }

// void releaseFor(ForLoop *forLoop) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (forLoop != NULL) {
// 		releaseForParams(forLoop->params);
// 		releaseCode(forLoop->body);
// 		free(forLoop);
// 	}
// }

// void releaseWhile(WhileLoop *whileLoop) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (whileLoop != NULL) {
// 		releaseExpression(whileLoop->condition);
// 		releaseCode(whileLoop->body);
// 		free(whileLoop);
// 	}
// }

// // Functions -------------------------------------------------------------------------------------------
// void releaseFunctionCall(FunctionCall *functionCall) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (functionCall != NULL) {
// 		free(functionCall->id);
// 		releaseValueList(functionCall->arguments);
// 	}
// }

// void releaseFunctionDeclaration(FunctionDeclaration *functionDeclaration) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (functionDeclaration != NULL) {
// 		free(functionDeclaration->id);
// 		releaseVariableTypeList(functionDeclaration->arguments);
// 		releaseCode(functionDeclaration->body);
// 		free(functionDeclaration);
// 	}
// }

// void releaseArrowFunction(ArrowFunction *arrowFunction) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (arrowFunction != NULL) {
// 		releaseVariableTypeList(arrowFunction->arguments);
// 		releaseCode(arrowFunction->body);
// 		free(arrowFunction);
// 	}
// }

// void releaseAysncFunction(AsyncFunction *asyncFunction) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (asyncFunction != NULL) {
// 		releaseVariableTypeList(asyncFunction->arguments);
// 		releaseCode(asyncFunction->body);
// 		free(asyncFunction);
// 	}
// }

// void releaseCode(Code *code) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (code != NULL) {
// 		switch (code->statement) {
// 			case IF_ST:
// 				releaseIf(code->ifStatement);
// 				break;
// 			case FOR_ST:
// 				releaseFor(code->forLoop);
// 				break;
// 			case WHILE_ST:
// 				releaseWhile(code->whileLoop);
// 				break;
// 			case FUNCTION_DECLARATION_ST:
// 				releaseFunctionDeclaration(code->FunctionDeclaration);
// 				break;
// 			case ARROW_FUNCTION_ST:
// 				releaseArrowFunction(code->arrowFunction);
// 				break;
// 			case ASYNC_FUNCTION_ST:
// 				releaseAysncFunction(code->asyncFunction);
// 				break;
// 			case DECLARATION_ST:
// 				releaseDeclaration(code->declaration);
// 				break;
// 			case EXPRESSION_ST:
// 				releaseExpression(code->expression);
// 				break;
// 			case VARIABLE_ST:
// 				releaseVariable(code->variable);
// 				break;
// 			case FUNCTIONCALL_ST:
// 				releaseFunctionCall(code->functionCall);
// 				break;
// 		}
// 		free(code);
// 	}
// }

// void releaseProgram(Program *program) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (program != NULL) {
// 		releaseCode(program->code);
// 		free(program);
// 	}
// }
