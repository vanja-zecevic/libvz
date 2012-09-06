/* Copyright (C) 2012 Vanja Zecevic
   Contact vanja.zecevic@sydney.uni.edu.au

   This file is part of lib_vz

   lib_vz is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lib_vz is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* =============================================================================
   lib_vz/gen_vz.c
                                  ----------------------------------------------

   This program is a source code pre-processor designed to repeat sections of
   code with some parameter changed. The directive,

    #pragma vzg split XXX [ x1 x2 x3 ... ] YYY [ y1 y2 y3 ... ] ...

   marks the beginning of a section to be repeated. XXX, YYY etc. represent
   variables to be changed during each repetition and are followed by a list of
   values to take enclosed in []. Where there are multiple variables, each
   possible permutation will be substituted.

   A preprocessor #define will be created for each value at each repetition. In
   order to help with naming functions, a helper macro is also created for each
   variable in order to append its current value to a string, eg.

    #define APND_XXX(IN_TOKEN) IN_TOKEN##_x1

   In order to assist with conditional compilation, numerical equivalents are
   also generated,

    #define XXX_NUM (0, 1, ... )

   The following directive marks the end of a repeated section.

    #pragma vzg splitend

   -----------------------------------------------------------------------------
   Example usage, compilation:
                                        ----------------------------------------

   In order to pre-process an input file in.gen.c,

    flex -s -o gen_vz.c gen_vz.lex
    gcc -Wall gen_vz.c -o gen_vz -lfl
    cat in.gen.c | gen_vz > in.c
     
   -----------------------------------------------------------------------------
   Example input file:
                                        ----------------------------------------

   ..:: in.gen.c ::..
    #pragma vzg split VARA [0 1 2] VARB [x y]
    void APND_VARA(APND_VARB(func_b_)) ( ... )
    {
    APND_VARA(APND_VARB(func_a_))  ( ... )
    }
    #pragma vzg splitend
 
*/
%option noyywrap
%option noinput
%option nounput
    typedef struct {
        int nvals;
        char * name;
        char ** vals;
    } tknlst; 

    int split         = 0;
    int splitdef      = 0;
    int split_ntokens = 0;
    int lvl           = 0;
    int buffer_leng   = 0;
    int read_val      = 0;
    char * buffer     = NULL;
    tknlst * tokens   = NULL;
    FILE * out_f;

    void exit_err(char * err_msg);
    void echo_redir();
    void yytext_to_buffer();
    void add_value_to_token();
    void add_new_token();
    void output_split();
    void write_recurs(int itok, int * valind);
    void reset_all();

%%
#pragma\ vzg\ split  { if (split==0) {
                           split=1;
                           splitdef=1;
                       }
                       else exit_err("Error: cannot nest 'split'."); }
#pragma\ vzg\ splitend { if (split==1) {
                           output_split();
                           reset_all();
                       }
                       else exit_err("Error: no 'split' to end."); }

 /* Exit splitdef mode at a newline that is not preceded by a
    \ character.  */
\\\n                 { if (splitdef==1);
                       else echo_redir(); }
\n                   { if (splitdef==1) splitdef=0;
                       else echo_redir(); }

 /* This rule increases the level during parsing.  */
\{                   { lvl++; echo_redir(); }
\}                   { lvl--; echo_redir(); }

 /* Set a flag if currently reading possible values for a token to take.  */
\[                   { if (splitdef==1) read_val = 1;
                       else echo_redir(); }
\]                   { if (splitdef==1) read_val = 0;
                       else echo_redir(); }

 /* Eat commas in 'split' definition.  */
,                    { if (splitdef==1);
                       else echo_redir(); }

 /* Add new token or add possible values to existing token.  */
[a-zA-Z0-9_]*        { if (splitdef==1) {
                           if(read_val==1)
                             add_value_to_token();
                           else
                             add_new_token();
                       }
                       else echo_redir(); }

 /* Eat spaces in 'split' and 'call' definitions.  */
[[:space:]]          { if (splitdef==1);
                       else echo_redir(); }

 /* This pattern catches everything else, exits if previous rules couldn't
    catch something in 'splitdef' section.  */
 
.                    { if (splitdef==1)
                         exit_err("Error: Unmatched char in splitdef.");
                       else echo_redir(); }
%%
/*============================================================================*/
/* Helper functions.  */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void echo_redir()
{
if (split==1) yytext_to_buffer(); 
else fprintf(out_f,"%s",yytext);
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void yytext_to_buffer()
{
char * realloc_tmp = NULL;
realloc_tmp = (char*)realloc(buffer, (buffer_leng+yyleng+1)*sizeof(char)); 

/* Make sure the new allocation succeeds.  */
if(realloc_tmp==NULL) exit_err("FAILURE: Out of memory 1.");
else buffer = realloc_tmp;

memcpy(buffer + buffer_leng, yytext, yyleng);
*(buffer + buffer_leng + yyleng) = '\0';
buffer_leng += yyleng;
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void exit_err(char * err_msg)
{
printf("%s\n", err_msg);
exit(1);
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void add_value_to_token()
{
int itok;
char ** realloc_tmp = NULL;
char * val_ptr;

itok = split_ntokens - 1;
(tokens + itok)->nvals++;

realloc_tmp = (char**)realloc((tokens + itok)->vals,
  ((tokens + itok)->nvals)*sizeof(char**)); 

/* Make sure the new allocation succeeds.  */
if(realloc_tmp==NULL) exit_err("FAILURE: Out of memory 2.");
else (tokens + itok)->vals = realloc_tmp;

val_ptr = (char*)malloc((yyleng+1)*sizeof(char));

if (val_ptr == NULL) exit_err("FAILURE: Out of memory 3.");
else {
    memcpy(val_ptr, yytext, yyleng);
    *(val_ptr + yyleng) = '\0';
}

*((tokens + itok)->vals + (tokens + itok)->nvals - 1) = val_ptr;

}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void add_new_token()
{
int itok;
tknlst * realloc_tmp = NULL;

itok = split_ntokens;
split_ntokens++;
realloc_tmp = (tknlst*)realloc(tokens, split_ntokens*sizeof(tknlst)); 

/* Make sure the new allocation succeeds.  */
if(realloc_tmp==NULL) exit_err("FAILURE: Out of memory 4.");
else tokens = realloc_tmp;

/* Initialize 'tokens' entry.  */
(tokens + itok)->nvals = 0;
(tokens + itok)->vals = NULL;
(tokens + itok)->name = (char*)malloc((yyleng+10)*sizeof(char));

if ((tokens + itok)->name == NULL) exit_err("FAILURE: Out of memory 5.");
else {
    memcpy((tokens + itok)->name, yytext, yyleng);
    *((tokens + itok)->name + yyleng) = '\0';
}
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void output_split()
{
int * valind; 

valind = (int*)malloc(split_ntokens*sizeof(int));
if (valind==NULL) exit_err("FAILURE: Out of memory 6.");
else write_recurs(0, valind);

}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void write_recurs(int itok, int * valind)
{
int idef;
int ival;

if(itok==split_ntokens) {
    for (idef=0; idef<split_ntokens; idef++)
      fprintf(out_f,"#define %s %s\n", (tokens + idef)->name,
        *((tokens + idef)->vals + *(valind + idef)) );
    for (idef=0; idef<split_ntokens; idef++)
      fprintf(out_f,"#define APND_%s(IN_TOKEN) IN_TOKEN##_%s\n",
        (tokens + idef)->name,
        *((tokens + idef)->vals + *(valind + idef)) );
    for (idef=0; idef<split_ntokens; idef++)
      fprintf(out_f,"#define %s_NUM %i\n", (tokens + idef)->name, *(valind + idef) );
    fprintf(out_f,"%s\n",buffer);
    for (idef=0; idef<split_ntokens; idef++)
      fprintf(out_f,"#undef %s\n", (tokens + idef)->name );
    for (idef=0; idef<split_ntokens; idef++)
      fprintf(out_f,"#undef APND_%s\n",
        (tokens + idef)->name );
    for (idef=0; idef<split_ntokens; idef++)
      fprintf(out_f,"#undef %s_NUM\n", (tokens + idef)->name );
    fprintf(out_f,"\n");
}
else for(ival=0; ival<(tokens + itok)->nvals; ival++) {
    *(valind + itok) = ival;
    write_recurs(itok+1, valind);
}

}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
void reset_all()
{
int itok;
int ival;

/* Free 'split' varables.  */
split=0;
for (itok=0; itok<split_ntokens; itok++) {
    free((tokens + itok)->name); 
    for (ival=0; ival<(tokens + itok)->nvals; ival++)
      free(*((tokens + itok)->vals + ival));
    free((tokens + itok)->vals);
}
free(tokens);
tokens = NULL;
split_ntokens = 0;

/* Free the buffer.  */
free(buffer);
buffer = NULL;
buffer_leng=0;
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
int main(int argc, char *argv[])
{
int iArgs = 0;
int outArg = 0;
/* Make sure at least an input and output file are specified.  */
if (argc<4) {
    printf("Usage gen_vz [-h] -o out_fname in_fname\n");
    return 1;
}
else {
    for (iArgs=1; iArgs<argc; iArgs++) {
        if(!strcmp(argv[iArgs],"-o")) break;
    }
}
outArg = iArgs;
/* Fail if -o didnt have 2 subsequent arguments.  */
if ( (argc-outArg)<=2 ) {
    printf("Usage gen_vz [-h] -o out_fname in_fname\n");
    return 1;
}
/* Open input and output files.  */
yyin = fopen( argv[argc-1], "r" );
out_f = fopen( argv[outArg+1], "w");

yylex();

return 0;
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

