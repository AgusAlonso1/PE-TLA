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

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

/* TypeScript to JavaScript Transpiler */

// Basic reusable patterns
crlf            \r\n
whitespace      [ \f\n\r\t\v]
identifier      [a-zA-Z_$][a-zA-Z0-9_$]* 
string          \"([^\\\"]|\\.)*\" 
number          [0-9]+(\.[0-9]+)? 
bool            (true|false) 
null            null 
undefined       undefined 
array           \[[^]]*\] 
jsonObject      \{([^{}]|(?R))*\} 

// TypeScript-specific patterns
function        function
return          return
const           const
let             let
var             var
if              if
else            else
for             for
while           while
do              do
switch          switch
case            case
break           break
continue        continue
try             try
catch           catch
finally         finally
async           async
await           await
class           class
extends         extends
implements      implements
interface        interface
public          public
private         private
protected       protected

// Type patterns
type            :\s*(string|number|boolean|any|unknown|never|void|\{[^}]*\}) //check
unionType      ({string}|{identifier}|\{identifier\}|\{type\}) 
stringLiteral   \"([^\\\"]|\\.)*\" 
enumIdentifier  {identifier} // Identificador de enum

// Arrow and async function patterns
arrowFunc       {identifier}\s*=>  // Función de flecha
params          \(\s*({identifier}(\s*:\s*{type})?\s*(,\s*{identifier}(\s*:\s*{type})?\s*)*)?\)

// Control structures and loops
forOf          for\s+({identifier}|{array})\s+of
neverType      never
anyType        any
unknownType    unknown

// Operators
arithmeticOp   (\+|\-|\*|\/|%) 
logicalOp      (&&|\|\|) 
relationalOp   (==|===|!=|!==|>|<|>=|<=)

%%

// Token definitions
{whitespace}      {} 
{string}          { return StringTokenPattern(yytext); }
{number}          { return NumberTokenPattern(yytext); }
{bool}            { return BooleanTokenPattern(yytext); }
{null}            { return NullTokenPattern(yytext); }
{undefined}       { return UndefinedTokenPattern(yytext); }
{array}           { return ArrayTokenPattern(yytext); }
{jsonObject}      { return JsonObjectPattern(yytext); }
{enumIdentifier}  { return EnumIdentifierPattern(yytext); }
{unionType}       { return UnionTypePattern(yytext); }

"function"        { return FunctionTokenPattern(yytext); }
"return"          { return ReturnTokenPattern(yytext); }
"const"           { return ConstTokenPattern(yytext); }
"let"             { return LetTokenPattern(yytext); }
"var"             { return VarTokenPattern(yytext); }
"if"              { return IfTokenPattern(yytext); }
"else"            { return ElseTokenPattern(yytext); }
"for"             { return ForTokenPattern(yytext); }
"while"           { return WhileTokenPattern(yytext); }
"do"              { return DoTokenPattern(yytext); }
"switch"          { return SwitchTokenPattern(yytext); }
"case"            { return CaseTokenPattern(yytext); }
"break"           { return BreakTokenPattern(yytext); }
"continue"        { return ContinueTokenPattern(yytext); }
"try"             { return TryTokenPattern(yytext); }
"catch"           { return CatchTokenPattern(yytext); }
"finally"         { return FinallyTokenPattern(yytext); }
"async"           { return AsyncTokenPattern(yytext); }
"await"           { return AwaitTokenPattern(yytext); }
"class"           { return ClassTokenPattern(yytext); }
"extends"         { return ExtendsTokenPattern(yytext); }
"implements"      { return ImplementsTokenPattern(yytext); }
"interface"       { return InterfaceTokenPattern(yytext); }
"public"          { return PublicTokenPattern(yytext); }
"private"         { return PrivateTokenPattern(yytext); }
"protected"       { return ProtectedTokenPattern(yytext); }
{params}          { return ParamsTokenPattern(yytext); }
{arrowFunc}       { return ArrowFunctionPattern(yytext); }
{arithmeticOp}    { return ArithmeticOperatorPattern(yytext); }
{logicalOp}       { return LogicalOperatorPattern(yytext); }
{relationalOp}    { return RelationalOperatorPattern(yytext); }

.                 { return UnknownPatternAction(yytext); }

%%


#include "FlexExport.h"
