%{
#include"parser.tab.h"
%}

ws [ \t]+
newline \n
comment [\/]{2}.*
letter [A-Za-z]
digit [0-9]
intkeyword int
voidkeyword void
floatkeyword float
whilekeyword while
dokeyword do
ifkeyword if
elsekeyword else
printkeyword print
returnkeyword return
gt >
gte >=
lt <
lte <=
nequal !=
equal ==
and &&
orgate \|\|
not !
id ({letter}|_)({letter}|{digit}|_)*
float {digit}*\.{digit}+|{digit}+\.{digit}*
integer {digit}+
metachar [\(\)\{\}\,;?:]
assign =
operator [\+\-\*/]
extra .

%%
{ws} { /*no action, no return*/}
{comment} { }
{newline} {//yylineno++; /*no action, no return*/
}

{printkeyword} {
store_token_name("PRINT",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return PRINT;  
}

{returnkeyword} {
store_token_name("RETURN",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return RETURN;  
}


{gt} {
store_token_name("GREATER_THAN",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return GREATER_THAN;  
           }
{gte} {
store_token_name("GREATER_THAN_EQUAL",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return GREATER_THAN_EQUAL;  
           }
{lt} {
store_token_name("LESS_THAN",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return LESS_THAN;  
           }

{lte} {
store_token_name("LESS_THAN_EQUAL",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return LESS_THAN_EQUAL;  
           }
{equal} {
store_token_name("EQUAL",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return EQUAL;  
           }
{nequal} {
store_token_name("NOT_EQUAL",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return NOT_EQUAL;  
           }
{and} {
store_token_name("AND",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return AND;  
           }
{orgate} {
store_token_name("OR",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return OR;  
           }
{not} {
store_token_name("NOT",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return NOT;  
           }
           
{metachar} {
store_token_name("META CHAR",yytext,yylineno);
       yylval.string_value =  new string(yytext);
       return *yytext;
       }
{whilekeyword} {  
store_token_name("WHILE",yytext,yylineno);
        yylval.string_value = new string(yytext);
        return WHILE;
        }
{dokeyword} {  
store_token_name("DO",yytext,yylineno);
        yylval.string_value = new string(yytext);
        return DO;
        }
{ifkeyword} {  
store_token_name("IF",yytext,yylineno);
        yylval.string_value = new string(yytext);
        return IF;
        }
{elsekeyword} {  
store_token_name("ELSE",yytext,yylineno);
        yylval.string_value = new string(yytext);
        return ELSE;
        }


{intkeyword} {  
store_token_name("INTEGER",yytext,yylineno);
        yylval.string_value = new string(yytext);
        return INTEGER;
        }
{voidkeyword} {
store_token_name("VOID",yytext,yylineno);
         yylval.string_value = new string(yytext);
        return VOID;
        }
{floatkeyword} {
store_token_name("FLOAT",yytext,yylineno);
         yylval.string_value = new string(yytext);
        return FLOAT;  
        }
{id} {
store_token_name("NAME",yytext,yylineno);

          yylval.string_value = new string(yytext);
          if(*yylval.string_value != "main"){
            *yylval.string_value = *yylval.string_value + "_";
          }
          return NAME;
          }
{float} {
store_token_name("FNUM",yytext,yylineno);
         yylval.double_value = atof(yytext);
        return DOUBLE_NUMBER;
        }
{integer} {
store_token_name("NUM",yytext,yylineno);
          yylval.integer_value = atoi(yytext);
         return INTEGER_NUMBER;
         }
{operator} {
store_token_name("ARITHOP",yytext,yylineno);
         yylval.string_value = new string(yytext); 
         return *yytext; 
        }
{assign} {
store_token_name("ASSIGN_OP",yytext,yylineno);
          yylval.string_value = new string(yytext);
            return ASSIGN;  
           }
{extra} {       
         fprintf(stderr,"cs316: Error : File: , Line: %d: scanner internal Error --no action found scanner internal Error--no action found\n",yylineno);  
         exit(EXIT_FAILURE);
}

%%
