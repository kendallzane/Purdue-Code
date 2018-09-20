
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token	<string_val> WORD
%token	<string_val> SUBCOMMAND

%token 	NOTOKEN GREAT NEWLINE GREATGREAT PIPE AMPERSAND GREATGREATAMPERSAND GREATAMPERSAND LESS

%union	{
		char   *string_val;
	}

%{
//#define yylex yylex
#define RED     "\x1b[31m"
#define RES     "\x1b[0m"

#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include "command.h"
#include <assert.h>
#include <unistd.h>

#define MAXFILENAME 1024

void yyerror(const char *);
void expandWildcardsIfNecessary(char *);
void sortArrayStrings(char **, int);
int compare (const void *, const void *);
void expandWildcard(char *,char *);
int yylex();

char ** array;
int maxEntries = 20;
int nEntries = 0;

%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	//command_and_args iomodifier_opt NEWLINE {
	pipe_list iomodifier_list background_optional NEWLINE {
		//printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word argument_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;

pipe_list:
	pipe_list PIPE command_and_args
	| command_and_args
	;

argument_list:
	argument_list argument
	| /* can be empty */
	;

argument:
	WORD {
              // printf("   Yacc: insert argument \"%s\"\n", $1);

			expandWildcardsIfNecessary($1);
	}
	;

command_word:
	WORD {
              // printf("   Yacc: insert command \"%s\"\n", $1);
	       
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;

iomodifier_opt:
	GREAT WORD {
		//printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
		//Command::_currentCommand._errFile = $2;
		Command::_currentCommand._numberOfOutputs++;
	}
	| GREATGREAT WORD {
		//printf("   Yacc: insert output (append) \"%s\"\n", $2);
		Command::_currentCommand._append = 1;
		Command::_currentCommand._outFile = $2;
		//Command::_currentCommand._errFile = $2;
		Command::_currentCommand._numberOfOutputs++;
	}
	| GREATGREATAMPERSAND WORD {
		//printf("   Yacc: insert output (append error) \"%s\"\n", $2);
		Command::_currentCommand._append = 1;
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._errFile = $2;
		Command::_currentCommand._numberOfOutputs++;
	}
	| GREATAMPERSAND WORD {
		//printf("   Yacc: insert output (error) \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._errFile = $2;
		Command::_currentCommand._numberOfOutputs++;
	}
	| LESS WORD {
		//printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inFile = $2;
		Command::_currentCommand._numberOfInputs++;
	}
	;

iomodifier_list:
	iomodifier_list iomodifier_opt
	| /*empty*/
	;

background_optional:
	AMPERSAND {
		Command::_currentCommand._background = 1;
	}
	| /*empty*/
	;

%%

void expandWildcardsIfNecessary(char * arg) {

	// Return if arg does not contain '*' or '?'
	if (strchr(arg, '*') == NULL && strchr(arg, '?') == NULL) {
		
		Command::_currentSimpleCommand->insertArgument(arg);
		return;
	}
	
	/*
	
	printf("expanding\n");
	// 1. Convert wildcard to regular expression
	
	char * reg = (char*)malloc(2*strlen(arg)+10);
	char * a = arg;
	char * r = reg;
	
	// match beginning of line
	*r = '^'; r++; 
	
	while (*a) {
		printf("found character: %c\n", *a);
		if (*a == '*') { *r='.'; r++; *r='*'; r++; }
		else if (*a == '?') { *r='.'; r++;}
		else if (*a == '.') { *r='\\'; r++; *r='.'; r++;}
		else { *r=*a; r++;}
		a++;
	}
	
	// match end of line and add null char
	*r='$'; r++; *r=0;	
	printf("regex string: %s\n", reg);
	
	// 2. compile regular expression.
	
	regex_t re;
	
	int result = regcomp( &re, reg, REG_EXTENDED|REG_NOSUB);
	if ( result != 0 ) {
		perror("compile");
		return;
	}
	regmatch_t match;
	
	// 3. List directory and add as arguments the entries
	// that match the regular expression
	
	DIR * dir = opendir(".");
	if (dir == NULL) {
		perror("opendir");
		return;
	}
	
	struct dirent * ent;
	int maxEntries = 20;
	int nEntries = 0;
	char ** array = (char**) malloc(1024*maxEntries*sizeof(char*));
	
	while ( (ent = readdir(dir))!= NULL) {
	// Check if name matches
		printf("checking filename: %s\n", ent->d_name);
		if (regexec(&re, ent->d_name, 1, &match, 0) == 0) {
			// Add argument
			printf("found match!\n");
			
			
			if (ent->d_name[0] == '.') {
				if (arg[0] == '.') {
					
					if (nEntries == maxEntries) {
						maxEntries *= 2;
						array = (char**)realloc(array, 1024*maxEntries*sizeof(char*));
						assert(array!=NULL);
					}
					array[nEntries]= strdup(arg);
				}
			} else {
				
				if (nEntries == maxEntries) {
					maxEntries *= 2;
					array = (char**)realloc(array, 1024*maxEntries*sizeof(char*));
					assert(array!=NULL);
				}
				array[nEntries]= strdup(ent->d_name);
				nEntries++;
			}
			//Command::_currentSimpleCommand->insertArgument(strdup(ent->d_name));
		}
	}
	
	closedir(dir);
	*/
	
	array = (char**) malloc(1024*maxEntries*sizeof(char*));
	
	//printf("beginning expansion\n");
	
	expandWildcard((char*)"", arg);
	//printf("finished expansion\n");
	
	if (nEntries == 0) {
		//printf("trying again\n");
		expandWildcard((char*)"", arg);
	}
	
	sortArrayStrings(array, nEntries);
	//printf("finished sort\n");
	
	for (int i = 0; i < nEntries; i++) { 
		Command::_currentSimpleCommand->insertArgument(array[i]);
	}
	
	//reset the array, make a new one
	free(array);
	maxEntries = 20;
	nEntries = 0;
	
}


void
sortArrayStrings(char ** array, int nEntries)
{
	if (nEntries > 1) {
		qsort(array, nEntries, sizeof(const char *), compare);
	}
}

int
compare (const void * a, const void * b)
{
	return strcmp(*(const char **)a, *(const char **)b);
}


void 
expandWildcard(char * prefix, char * suffix)
{
	//printf("got here\n");
	//printf("prefix: %s	suffix: %s\n", prefix, suffix);
	
	if(array == NULL) {
		array = (char**)malloc(1024*maxEntries*sizeof(char*));
	}
	
	if (suffix[0] == 0) {
		//printf("suffix is empty. Put prefix in argument\n");
		// suffix is empty. Put prefix in argument.
		//Command::_currentSimpleCommand->insertArgument(strdup(prefix));
		
		if (nEntries == maxEntries) {
			maxEntries *= 2;
			array = (char**)realloc(array, 1024*maxEntries*sizeof(char*));
			assert(array!=NULL);
		}
		
		
		array[nEntries] = strdup(prefix);
		nEntries++;
		return;
	}
	
	// Obtain the next component in the suffix
	// Also advance suffix.
	
	char * s = strchr(suffix, '/');
	char component[MAXFILENAME];
	memset(component, 0, sizeof component);
	//printf("s: %s\n", s);
	if (suffix[0] != '/') {
		//printf("suffix[0] = %C\n", suffix[0]);
		if (s != NULL) { // Copy up to the first "/"
			strncpy(component, suffix, s - suffix);
			suffix = s + 1;
		} else { // Last part of the path. Copy whole thing.
			strcpy(component, suffix); 
			suffix = suffix + strlen(suffix); 
		}
	} else {
		//printf("suffix[0] = '/', I think\n");
		suffix = s+1;
	}
	
	//printf("component: %s\n", component);
	//printf("new suffix: %s\n", suffix);
	 
	// Now we need to expand the component 
    char newPrefix[MAXFILENAME]; 
    if (strchr(component, '*') == NULL && strchr(component, '?') == NULL) { 
        //printf("component does not have wildcards\n"); 
        //s//printf(newPrefix,"%s/%s", prefix, component); 
		if (prefix == "" || strcmp(prefix, "/") == 0){
			//printf("prefix is null or / \n");
			sprintf(newPrefix,"%s%s", prefix, component);
		} else {
			sprintf(newPrefix,"%s/%s", prefix, component);
		}
		if (strcmp(prefix, "") == 0 && strcmp(newPrefix, "") == 0) {
			sprintf(newPrefix, "/");
		}
        expandWildcard(newPrefix, suffix); 
        return; 
    }
	
	// Component has wildcards
	// Convert component to regular expression
	
	
	// 1. Convert wildcard to regular expression
	
	char * reg = (char*)malloc(2*strlen(component)+10);
	char * a = component;
	char * r = reg;
	
	// match beginning of line
	*r = '^'; r++; 
	
	while (*a) {
		//printf("found character: %c...   ", *a);
		if (*a == '*') { *r='.'; r++; *r='*'; r++; }
		else if (*a == '?') { *r='.'; r++;}
		else if (*a == '.') { *r='\\'; r++; *r='.'; r++;}
		else { *r=*a; r++;}
		a++;
	}
	
	// match end of line and add null char
	*r='$'; r++; *r=0;	
	//printf("regex string: %s\n", reg);
	//printf("prefix: %s	suffix: %s  component: %s\n", prefix, suffix, component);
	// 2. compile regular expression.
	
	regex_t re;
	
	int result = regcomp( &re, reg, REG_EXTENDED|REG_NOSUB);
	if ( result != 0 ) {
		perror("compile");
		return;
	}
	regmatch_t match;
	
	char * dir;
	
	// If prefix is empty then list prefix
	
	if (prefix[0] == 0) {
		dir = (char *)".";
	} else {
		dir = (char *)prefix;
	}
	
	//printf("dir: %s\n", dir);
	
	DIR * d ;
	d = opendir(dir);
	//printf("good so far\n");
	if (d == NULL) {
		//printf("not found\n");
		return;
	}
	//printf("good so far\n");
	struct dirent * ent;
	//printf("good so far\n");
	// Now we need to check what entries match
	while ((ent = readdir(d))!= NULL) {
		
		
		//printf("checking file: %s\n", ent->d_name);
		if (regexec(&re, ent->d_name, 1, &match, 0) == 0) {
			//printf("---found match!		%s\n", ent->d_name);
			//printf("current prefix: %s\n", prefix);
			if (ent->d_name[0] == '.') {
				if (component[0] == '.') {
					if (prefix == "" || strcmp(prefix, "/") == 0){
						sprintf(newPrefix,"%s%s", prefix, ent->d_name);
					} else {
						sprintf(newPrefix,"%s/%s", prefix, ent->d_name);
					}
					
					expandWildcard(newPrefix,suffix);
					
				}
			} else {
		
				// Entry matches. Add name of entry
				// that matches to the prefix and
				// call expandWildcard(..) recursively
				if (prefix == "" || strcmp(prefix, "/") == 0){
					sprintf(newPrefix,"%s%s", prefix, ent->d_name);
				} else {
					sprintf(newPrefix,"%s/%s", prefix, ent->d_name);
				}
				
				expandWildcard(newPrefix,suffix);
			}
		}
	}
	closedir(d);
	free(reg);
	memset(component, 0, sizeof component);
	
	
}


void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
