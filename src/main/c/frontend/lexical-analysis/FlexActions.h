#ifndef LEXER_ACTIONS_HEADER
#define LEXER_ACTIONS_HEADER

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
void initializeLexerActionsModule();

/** Shutdown module's internal state. */
void shutdownLexerActionsModule();

/**
 * Lexer lexeme processing actions.
 */

// Handles basic types: null, undefined, boolean, string, and number
Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token NumberLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BooleanLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token NullLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token UndefinedLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// Array and JSON object handling
Token ArrayLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token JsonObjectLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// TypeScript-specific tokens
Token FunctionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token AsyncFunctionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ArrowFunctionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// Handling variables and declarations
Token ConstLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token LetLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token VarLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// Handling operators (arithmetic, logical, relational)
Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token LogicalOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token RelationalOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// Control structures
Token IfLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ForOfLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SwitchLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CaseLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BreakLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ContinueLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// TypeScript-specific types (never, any, unknown, union, enum)
Token NeverTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token AnyTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token UnknownTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token UnionTypeLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EnumLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// Handling parentheses and brackets (for expressions, arrays, function calls)
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token BracketLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

// Special lexemes (comments, ignored tokens, unknown tokens)
void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif

