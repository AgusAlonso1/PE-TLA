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

void EndSentenceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
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
Token IdentifierLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->id = lexicalAnalyzerContext->lexeme;
	return ID;
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

Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->value_int = atoi(lexicalAnalyzerContext->lexeme);
	return INT_VALUE;
}

Token NumberLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->value_float = atof(lexicalAnalyzerContext->lexeme);
	return FLOAT_VALUE;
}

Token BooleanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	if (strcmp(lexicalAnalyzerContext->lexeme, "true") == 0) {
		lexicalAnalyzerContext->semanticValue->value_bool = 1; // Boolean true
	}
	else if (strcmp(lexicalAnalyzerContext->lexeme, "false") == 0) {
		lexicalAnalyzerContext->semanticValue->value_bool = 0; // Boolean false
	}
	return STRING_VALUE;
}

Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->value_str = lexicalAnalyzerContext->lexeme;
	return BOOL_VALUE;
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
	lexicalAnalyzerContext->semanticValue->single_type = malloc(strlen(lexicalAnalyzerContext->lexeme) + 1);
	stpcpy(lexicalAnalyzerContext->semanticValue->single_type, lexicalAnalyzerContext->lexeme);
	return SINGLE_TYPE;
}

// Keywords ------------------------------------------------------------------------------------------
Token KeywordLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}
