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
%x VARIABLE_TYPE
%x UNION_VARIABLES
%x ENUM
%x INTERFACE
%x IF 
%x FOR
%x SWITCH
%x FUNCTION


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
array           \[[^]]*\
jsonObject      \{([^{}]|(?R))*\}
any 			(string|number|boolean|jsonObject)

"-"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

">"                                                                         { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER); }
"<"                                                                         { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS); }
"="                                                                         { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), ASSING); }
"=="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), EQUAL); }               
"!="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), NEQUAL); }
"==="                                                                       { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), STRICT_EQUAL); }
"!=="                                                                       { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), STRICT_NEQUAL); } 
"&&"                                                                        { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }
"||"                                                                        { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), OR); }
"<="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS_EQUAL); }
">="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER_EQUAL); }

"("									                                        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									                                        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+						                                        { return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									                                        { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

"//"
<SINGLE_LINE_COMMENT>                                                       { BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>"\n"                                                   { BEGIN(INITIAL); }

"/*"								                                        { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				                                        { EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

//variables
"(let )|(const )"                                                           { BEGIN(VARIABLE_DECLARATION); return VariableModificatorLexemeAction(createLexicalAnalyzerContext()); } // If a "let " or "const " match, it creates a VARIABLE_DECLARATION context and a VariableModificator token. 
<VARIABLE_DECLARATION>[[:space:]]+                                          { IgnoredLexemeAction(createLexicalAnalyzerContext()); } // Any spaces in the middle are ignored.
<VARIABLE_DECLARATION>"(string|number|boolean|jsonObject|any|undefined)"    { return SingleTypeLexemeAction(createLexicalAnalyzerContext()); } 
<VARIABLE_DECLARATION>"="                                                   { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); } // Direct assignment operator token
<VARIABLE_DECLARATION>";"                                                   { EndSentenceLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL) } // If a ";" match, it ends the VARIABLE_DECLARATION context.

":\s*"                     	
<VARIABLE_TYPE>"(string|number|boolean|jsonObject|any|undefined)"           { return TypeLexemeAction(createLexicalAnalyzerContext()); } // If a ":" match, it creates a VARIABLE_TYPE context and a Type token.

"(|)?\s*(string|number|boolean|any|jsonObject|any|undefined)"
<UNION_VARIABLES>"(|\s*(string|number|boolean||any|undefined))*"            { return SingleTypeLexemeAction(createLexicalAnalyzerContext()); } //salteo
<UNION_VARIABLES>"="                                                        { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL);}

"enum "
<ENUM>[[:space:]]+                                                          { IgnoredLexemeAction(createLexicalAnalyzerContext()); } // Any spaces in the middle are ignored.
<ENUM>"\{"                                                                  { BEGIN(ENUM) }
<ENUM>":\s*(string|number)" 		                                        { return EnumLexemeAction(createLexicalAnalyzerContext());}
<ENUM>","                                                                   { } //append 
<ENUM>"\}"                                                                  { BEGIN(INITIAL);}

"interface "
<INTERFACE>[[:space:]]+                                                     { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<INTERFACE>"[a-zA-Z_$][a-zA-Z0-9_$]*"                                       { char *interfaceName = strdup(yytext); return BeginInterfaceLexemeAction(createLexicalAnalyzerContext(), interfaceName); } // check interface name param
<INTERFACE>"\{"                                                             { BEGIN(INTERFACE); }
<INTERFACE>"\}"                                                             { return EndInterfaceLexemeAction(); BEGIN(INITIAL); }

"if "
<IF>[[:space:]]+                                                            { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<IF>"\("                                                                    { return IfLexemeAction(createLexicalAnalyzerContext()); }
<IF>"\)"                                                                    { return EndIfConditionLexemeAction(createLexicalAnalyzerContext,); }
<IF>"\{"                                                                    { BEGIN(IF); }
<IF>"\}"                                                                    { return EndIfLexemeAction(); BEGIN(INITIAL); }
<IF>"else"                                                                  { return ElseLexemeAction(createLexicalAnalyzerContext()); }

"for "
<FOR>[[:space:]]+                                                           { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<FOR>"\("                                                                   { return ForLexemeAction(createLexicalAnalyzerContext()); }
<FOR>"\)"                                                                   { return EndForConditionLexemeAction(createLexicalAnalyzerContext); }
<FOR>"\{"                                                                   { BEGIN(FOR); }
<FOR>"\}"                                                                   { return EndForLexemeAction(); BEGIN(INITIAL); }


"function "
<FUNCTION>[[:space:]]+                                                      { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<FUNCTION>"[a-zA-Z_$][a-zA-Z0-9_$]*"                                        { char *functionName = strdup(yytext); return BeginFunctionLexemeAction(createLexicalAnalyzerContext(), functionName); } // check function name param
<FUNCTION>"\("						                                        { BEGIN(FUNCTION); return FunctionLexemeAction(createLexicalAnalyzerContext()); }
<FUNCTION>":\s*(string|number|boolean|any|undefined)"	                    { TypeLexemeAction(createLexicalAnalyzerContext,)} // despues se declara el tipo de retorno
<FUNCTION>"," 					 	                                        { } //append variables 
<FUNCTION>"\)"                                                              { return FunctionParamLexemeAction(); BEGIN(INITIAL); }
<FUNCTION>"\{"                                                              { BEGIN(FUNCTION); }
<FUNCTION>"\}"                                                              { return EndFunctionParamLexemeAction(); BEGIN(INITIAL); }




%%

#include "FlexExport.h"
