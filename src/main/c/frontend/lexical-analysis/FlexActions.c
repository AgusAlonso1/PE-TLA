#include "FlexActions.h"
#include "LexicalAnalyzerContext.h"

#define FIRST_CHAR_POS 0

/* MODULE INTERNAL STATE */

static Logger *_logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char *functionName, LexicalAnalyzerContext *lexicalAnalyzerContext) {
	char *escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
				 functionName,
				 escapedLexeme,
				 lexicalAnalyzerContext->currentContext,
				 lexicalAnalyzerContext->length,
				 lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token VariableModificatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	lexicalAnalyzerContext->semanticValue->token = lexicalAnalyzerContext->lexeme[FIRST_CHAR_POS]; // l or c => let or const
	return VARIABLE_MODIFICATOR;
}

void EndSentenceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token DirectAssignmentOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return DIRECT_ASSIGNMENT;
}

void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return UNKNOWN;
}

// ID -----------------------------------------------------------------------------------------------
void IdentifierLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->id = lexicalAnalyzerContext->lexeme;
}

// Operators ----------------------------------------------------------------------------------------
Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token ComparisonOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token LogicalOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token AssignmentOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}
Token ValueLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->value = lexicalAnalyzerContext->lexeme;
	return VALUE;
}

// Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
// 	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
// 	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
// 	return INTEGER;
// }

// Delimiters ----------------------------------------------------------------------------------------
Token DelimiterLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

// Variable declaration ------------------------------------------------------------------------------
Token VariableDeclarationLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

// Types ---------------------------------------------------------------------------------------------
Token SingleTypeLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->type = lexicalAnalyzerContext->lexeme;
	return SINGLE_TYPE;
}

// Keywords ------------------------------------------------------------------------------------------
Token KeywordLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

//error ----
Token ErrorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext){
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	
	char *errorStart = strstr(LexicalAnalyzerContext->lexeme, ", ") + 2;
    char *errorEnd = strstr(errorStart, ")");

    int errorLength = errorEnd - errorStart;
	char *error;
    strncpy(error, errorStart, errorLength);
    error[errorLength] = '\0';

	lexicalAnalyzerContext->error = error;

	return CONSOLE_ERROR;
}
