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
%x SINGLE_LINE_COMMENT
%x MULTILINE_COMMENT
%x VARIABLE_DECLARATION
%x UNION_VARIABLES
%x ENUM

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

// Data types
identifier      [a-zA-Z_$][a-zA-Z0-9_$]* 
string          \"([^\\\"]|\\.)*\" 
number          [0-9]+(\.[0-9]+)? 
boolean         (true|false) 
null            null 
undefined       undefined 
array           \[[^]]*\] 
jsonObject      \{([^{}]|(?R))*\}
any 			(string|number|boolean|jsonObject)

"//"
<SINGLE_LINE_COMMENT>               { BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>"\n"           { BEGIN(INITIAL); }

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

//variables
"(let )|(const )"                   { BEGIN(VARIABLE_DECLARATION); return VariableModificatorLexemeAction(createLexicalAnalyzerContext()); } // If a "let " or "const " match, it creates a VARIABLE_DECLARATION context and a VariableModificator token. 
<VARIABLE_DECLARATION>[[:space:]]+  { IgnoredLexemeAction(createLexicalAnalyzerContext()); } // Any spaces in the middle are ignored.
<VARIABLE_DECLARATION>"(string|number|boolean|jsonObject|any)" { }
<VARIABLE_DECLARATION>"="           { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); } // Direct assignment operator token
<VARIABLE_DECLARATION>";"           { EndSentenceLexemeAction(createLexicalAnalyzerContext); BEGIN(INITIAL) } // If a ";" match, it ends the VARIABLE_DECLARATION context.

"(|)?\s*(string|number|boolean|any|jsonObject)"
<UNION_VARIABLES>"(|\s*(string|number|boolean|any))*" { } //salteo
<UNION_VARIABLES>"="                { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL);}

"enum "
<ENUM>[[:space:]]+                  { IgnoredLexemeAction(createLexicalAnalyzerContext()); } // Any spaces in the middle are ignored.
<ENUM>"\{"                          { BEGIN(ENUM) }
<ENUM>","                           { } //append 
<ENUM>"\}"                          { BEGIN(INITIAL);}



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
