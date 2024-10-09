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
%x IDENTIFIER_TYPE
%x UNION_TYPE
%x ENUM_TYPE
%x INTERFACE
%x IF 
%x FOR
%x FUNCTION

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

// Data types
identifier      [a-zA-Z_$][a-zA-Z0-9_$]* 
string          \"([^\\\"]|\\.)*\" 
number          [0-9]+(\.[0-9]+)? 
boolean         (true|false) 
null            null 
undefined       undefined 
never 		 	never
unknown 		unknown
any 		   	any
void 		  	void
array           \[[^]]*\
jsonObject      \{([^{}]|(?R))*\}
types           (string|number|boolean|jsonObject|any|undefined|null)

%%

// Arithmetic Operators
"-"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									                                        { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

// Comparison Operators
">"                                                                         { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER); }
"<"                                                                         { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS); }
"=="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), EQUAL); }               
"!="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), NEQUAL); }
"==="                                                                       { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), STRICT_EQUAL); }
"!=="                                                                       { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), STRICT_NEQUAL); } 
"<="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), LESS_EQUAL); }
">="                                                                        { return ComparisonOperatorLexemeAction(createLexicalAnalyzerContext(), GREATER_EQUAL); }

// Logical Operators
"&&"                                                                        { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }
"||"                                                                        { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), OR); }

// Delimiters
"("									                                        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									                                        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+						                                        { return IntegerLexemeAction(createLexicalAnalyzerContext()); }

[[:space:]]+						                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"//"                                                                        { BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>[^\n]*			                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); } 
<SINGLE_LINE_COMMENT>.				                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_LINE_COMMENT>"\n"                                                   { BEGIN(INITIAL); }

"/*"								                                        { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[[:space:]]+		                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				                                        { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				                                        { EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }

// Variable declaration 
"(let )|(const )"                                                           { BEGIN(VARIABLE_DECLARATION); return VariableModificatorLexemeAction(createLexicalAnalyzerContext()); } 
<VARIABLE_DECLARATION>[[:space:]]+                                          { IgnoredLexemeAction(createLexicalAnalyzerContext()); } 
<VARIABLE_DECLARATION>{identifier}                                          { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
<VARIABLE_DECLARATION>"="                                                   { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); } 
<VARIABLE_DECLARATION>":"                                                   { return TypeDeclarationLexemeAction(createLexicalAnalyzerContext()); } 
<VARIABLE_DECLARATION>{types}                                               { return SingleTypeLexemeAction(createLexicalAnalyzerContext()); } 
<VARIABLE_DECLARATION>";"                                                   { EndSentenceLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL) } 

// Types declaration
<IDENTIFIER_TYPE>{identifier}												{ return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
<IDENTIFIER_TYPE>[[:space:]]+                                               { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<IDENTIFIER_TYPE>":"                                                        { return TypeDeclarationLexemeAction(createLexicalAnalyzerContext()); }
<IDENTIFIER_TYPE>[[:space:]]+                                               { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<IDENTIFIER_TYPE>{types}                                                    { return SingleTypeLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL);}

"(|)?\s*"+{types}
<UNION_TYPE>"(|\s*"+{types}*                                                { return MultipleTypeLexemeAction(createLexicalAnalyzerContext()); }
<UNION_TYPE>"="                                                             { return DirectAssignmentOperatorLexemeAction(createLexicalAnalyzerContext()); }


"enum"																		{ BEGIN(ENUM); return EnumLexemeAction(createLexicalAnalyzerContext());}
<ENUM_TYPE>[[:space:]]+                                                     { IgnoredLexemeAction(createLexicalAnalyzerContext()); } 
<ENUM_TYPE>{identifier}                                                     { return IdentifierLexemeAction(createLexicalAnalyzerContext()); } 
<ENUM_TYPE>"\{"                                                             { return BracesLexemeAction(OPEN_BRACES); BEGIN(ENUM) }

<ENUM_TYPE>"=" 		                                                        { return EnumLexemeAction(createLexicalAnalyzerContext());}
<ENUM_TYPE>","                                                              { } //append 
<ENUM_TYPE>"\}"                                                             { return BracesLexemeAction(CLOSE_BRACES); BEGIN(INITIAL);}

"interface"                                                                 { BEGIN(INTERFACE); }
<INTERFACE>[[:space:]]+                                                     { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<INTERFACE>{identifier}                                                     { return BracesLexemeAction(OPEN_BRACES); return IdentifierLexemeAction(createLexicalAnalyzerContext()); }
<INTERFACE>"\{"                                                             { BEGIN(IDENTIFIER_TYPE); }
<INTERFACE>"(,|;)"                                                    		{ BEGIN(IDENTIFIER_TYPE); }
<INTERFACE>"\}"                                                             { return BracesLexemeAction(CLOSE_BRACES); BEGIN(INITIAL); }

//PREGUNTAR
//Comparison
//else 
"if"                                                                        { BEGIN(IF); }
<IF>[[:space:]]+                                                            { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<IF>"\("                                                                    { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
<IF>"[^()]*"                                                                { return ConditionLexemeAction(createLexicalAnalyzerContext()); }
<IF>"\)"                                                                    { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS);  }
<IF>"\{"                                                              		{ return BracesLexemeAction(OPEN_BRACES); }
<IF>"\}"                                                             		{ return BracesLexemeAction(CLOSE_BRACES); BEGIN(INITIAL);}
<IF>"else"                                                                  { return ElseLexemeAction(createLexicalAnalyzerContext()); }

"for"                                                                       { BEGIN(FOR); }
<FOR>[[:space:]]+                                                           { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<FOR>"\("                                                                   { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }

<FOR>"(let|const)\\s+[a-zA-Z_$][a-zA-Z0-9_$]*\\s*=\\s*[0-9]+(\\.[0-9]+)?;"  { return ForInitLexemeAction(createLexicalAnalyzerContext()); }
<FOR>"([^;]+);"                                                             { return ConditionLexemeAction(createLexicalAnalyzerContext()); }

<FOR>"\)"                                                                   { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
<FOR>"\{"                                                                   { return BracesLexemeAction(OPEN_BRACES); }
// analizar lo de adentro
<FOR>"\}"                                                                   { return BracesLexemeAction(CLOSE_BRACES); BEGIN(INITIAL); }


"function "                                                                 { BEGIN(FUNCTION); }
<FUNCTION>[[:space:]]+                                                      { IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<FUNCTION>{identifier}                                                      { return IdentifierLexemeAction(createLexicalAnalyzerContext()); } 
<FUNCTION>"\("						                                        { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
<FUNCTION>{identifier}	                                                    { BEGIN(IDENTIFIER_TYPE) } // despues se declara el tipo de retorno
<FUNCTION>"(,|;)" 					 	                                    { BEGIN(IDENTIFIER_TYPE) } //append variables 
<FUNCTION>"\)"                                                              { return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
//return type PREGUNTAR
<FUNCTION>{types}  													        { return SingleTypeLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<FUNCTION>"\{"                                                              { return BracesLexemeAction(OPEN_BRACES); }
<FUNCTION>"\}"                                                              { return BracesLexemeAction(CLOSE_BRACES); BEGIN(INITIAL); }


.									                                        { return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
