%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x VARIABLE_DECLARATION
%x UNION_TYPE

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"(let )|(const )"                   { BEGIN(VARIABLE_DECLARATION); return VariableModificatorLexemeAction(createLexicalAnalyzerContext()); } // If a "let " or "const " match, it creates a VARIABLE_DECLARATION context and a VariableModificator token. 
<VARIABLE_DECLARATION>[[:space:]]+  { IgnoredLexemeAction(createLexicalAnalyzerContext()); } // Any spaces in the middle are ignored.
<VARIABLE_DECLARATION>"="           { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); } // Direct assignment operator token
<VARIABLE_DECLARATION>";"           { EndSentenceLexemeAction(createLexicalAnalyzerContext); BEGIN(INITIAL); } // If a ";" match, it ends the VARIABLE_DECLARATION context.

""
<UNION_TYPE>[[:space:]]+            { IgnoredLexemeAction(createLexicalAnalyzerContext()); } // Any spaces in the middle are ignored.<
<UNION_TYPE>"="                     { BEGIN(INITIAL); }

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
