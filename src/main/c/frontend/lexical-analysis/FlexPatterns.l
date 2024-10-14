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

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

/* Data types */
ID              [a-zA-Z_$][a-zA-Z0-9_$]* 
string          \"([^\\\"]|\\.)*\" 
number          [0-9]+(\.[0-9]+)? 
boolean         (true|false) 
%%

"//"                                                                        				{ BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>[^\n]*			                                        				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); } 
<SINGLE_LINE_COMMENT>"\n"                                                   				{ BEGIN(INITIAL); }

"/*"								                                        				{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[[:space:]]+		                                        				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			                                        				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				                                        				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				                                        				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }

"(string|number|boolean|any|undefined|null|void|unknown|never)(\\[\\])?"  					{ return SingleTypeLexemeAction(createLexicalAnalyzerContext()); }

"-"									                                        				{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									                                        				{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									                                        				{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									                                       					{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

">"                                                                         				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER); }
"<"                                                                         				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS); }
"=="                                                                        				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), EQUAL); }               
"!="                                                                        				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), NEQUAL); }
"==="                                                                       				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), STRICT_EQUAL); }
"!=="                                                                       				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), STRICT_NEQUAL); } 
"<="                                                                        				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS_EQUAL); }
">="                                                                        				{ return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER_EQUAL); }

"&&"                                                                        				{ return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }
"||"                                                                        				{ return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), OR); }

"="                                                                                         { return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), ASSIGN); }
"++"                                                                        				{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), INCREMENT); }
"--"                                                                        				{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), DECREMENT); }
"!"																							{ return AssignmentOperatorLexemeAction(createLexicalAnalyzerContext(), NOT); }

"[" 																						{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACKET); }
"]"                                                                         				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACKET); }
"("									                                        				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									                                        				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
"{"									                                        				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }		
"}"									                                        				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
","									                                        				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), COMA); }
";"									                                        				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), SEMI_COLON); }
":"									                                       	 				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), COLON); }
"|"                                                                         				{ return DelimiterLexemeAction(createLexicalAnalyzerContext(), PIPE); }

"const"                                                                     				{ return VariableDeclarationLexemeAction(createLexicalAnalyzerContext(), CONST); }
"let"                                                                       				{ return VariableDeclarationLexemeAction(createLexicalAnalyzerContext(), LET); }
"var"                                                                       				{ return VariableDeclarationLexemeAction(createLexicalAnalyzerContext(), VAR); }
"type"                                                                      				{ return VariableDeclarationLexemeAction(createLexicalAnalyzerContext(), TYPE); }
"if"									                                    				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), IF); } 
"for"                                                                       				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FOR); }
"while"                                                                     				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), WHILE); }
"else"                                                                      				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ELSE); }
"function"                                                                  				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), FUNCTION); }
"interface"                                                                 				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), INTERFACE); }
"enum"                                                                      				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ENUM); }
"of"                                                                        				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), OF); }
"=>" 																	   					{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ARROW); }
"async"                                                                     				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), ASYNC); }
"Promise"                                                                   				{ return KeywordLexemeAction(createLexicalAnalyzerContext(), PROMISE); }
"await" 																   					{ return KeywordLexemeAction(createLexicalAnalyzerContext(), AWAIT); }
[[:digit:]]+						                                        				{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
{number} 																 					{ return NumberLexemeAction(createLexicalAnalyzerContext()); }
{string}																					{ return StringLexemeAction(createLexicalAnalyzerContext()); }
{boolean}																					{ return BooleanLexemeAction(createLexicalAnalyzerContext()); }
{ID}                                                                        				{ return IdentifierLexemeAction(createLexicalAnalyzerContext()); }


[[:space:]]+						                                        				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									                                        				{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
