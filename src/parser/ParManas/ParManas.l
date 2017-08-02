%{

#include "util/Common.hpp"
#include <ParManas_parser.hpp>

%}

%option noyywrap
%option caseless
%option yylineno

%x C_COMMENT

%%

[ \t]+											{}
[\r\n\f]+[ \t]+[\r\n\f]+		{return NEWLINE;}
[\r\n\f]+										{return NEWLINE;}

"/*"												{BEGIN(C_COMMENT);}
<C_COMMENT>"*/"							{BEGIN(INITIAL);}
<C_COMMENT>[\r\n\f]					{}
<C_COMMENT>.								{}
\/\/.*[\r\n\f]+							{}
#.*[\r\n\f]+								{}

.DATA												{return DATA_SEG;}
.CODE												{return CODE_SEG;}

"$"[A-Z_][A-Z0-9_]*					{
															yylval.text = strdup(yytext+1);
															return VARIABLE;
														}

ZEROS												{return ZEROS;}
ONES												{return ONES;}

CELL												{return CELL;}
DPU													{return DPU;}
REFI_?1											{return REFI1;}
REFI_?2											{return REFI2;}
REFI_?3											{return REFI3;}
DELAY												{return DELAY;}
RACCU												{return RACCU;}
LOOP_?HEADER								{return LOOPHEADER;}
LOOP_?TAIL									{return LOOPTAIL;}
SWB													{return SWB;}
BRANCH											{return BRANCH;}
JUMP												{return JUMP;}
SRAM_?READ									{return SRAMREAD;}
SRAM_?WRITE									{return SRAMWRITE;}
ROUTE												{return ROUTE;}
HALT												{return HALT;}

LATA												{
															yylval.IntegerValue = 1;
															return INTEGER;
														}
REFI												{
															yylval.IntegerValue = 0;
															return INTEGER;
														}
0[0-9]*											{
															yylval.IntegerValue = strtol(yytext, NULL, 8);
															return INTEGER;
														}
[1-9][0-9]*									{
															yylval.IntegerValue = strtol(yytext, NULL, 10);
															return INTEGER;
														}
0X[0-9]*										{
															yylval.IntegerValue = strtol(yytext, NULL, 16);
															return INTEGER;
														}

EVEN_DISTR									{
															yylval.IntegerValue = 1;
															return EVEN_DISTR;
														}
FULL_DISTR									{
															yylval.IntegerValue = 0;
															return FULL_DISTR;
														}

"+"													{return ADD;}
"-"													{return SUB;}
"*"													{return MUL;}
"/"													{return DIV;}
"^"													{return POW;}
"("													{return PAL;}
")"													{return PAR;}
"["													{return BRACKET_L;}
"]"													{return BRACKET_R;}
"<"													{return SHEVRON_L;}
">"													{return SHEVRON_R;}
","													{return COMMA;}

.														{return LEXERROR;}

%%

