#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token VariableModificatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
void EndSentenceLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token NumberLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token BooleanLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token ComparisonOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token LogicalOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token AssignmentOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token ValueLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token SingleTypeLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token KeywordLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token DelimiterLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token VariableDeclarationLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token IdentifierLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

#endif
