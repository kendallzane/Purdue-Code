
//シェル - The Ugly Shell
//written by Kendall Z Bowles
//but mostly by Gustavo I'm assuming

//known issues:

//sending a command like "echo" or "ls" to the background will cause
//that commands output to appear in the shell. not sure where it would
//go otherwise though!

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

#include "command.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BBLACK     	   "\x1b[40m"

int firstTime = 1;

extern char **environ;

int yyparse(void);
int numCommands = 0;

SimpleCommand::SimpleCommand()
{
	// Create available space for 5 arguments
	_numOfAvailableArguments = 5;
	_numOfArguments = 0;
	_arguments = (char **) malloc( _numOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numOfAvailableArguments == _numOfArguments  + 1 ) {
		// Double the available space
		_numOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numOfAvailableArguments * sizeof( char * ) );
	}
	
	_arguments[ _numOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numOfArguments + 1] = NULL;
	
	_numOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
	_append = 0;
	_numberOfInputs = 0;
	_numberOfOutputs = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numOfAvailableSimpleCommands == _numOfSimpleCommands ) {
		_numOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
			 _numOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}
	
	_simpleCommands[ _numOfSimpleCommands ] = simpleCommand;
	_numOfSimpleCommands++;
}

void
Command:: clear()
{
	//printf("clearing\n");
	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}
		
		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	//int beCareful = 0;
	//printf("%s\n", _outFile);
	//printf("%s\n", _errFile);
	//if (strcmp(_outFile, _errFile) == "") {
	//	beCareful = 1;
	//}
	
	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inFile ) {
		free( _inFile );
	}

	if ( _errFile ) {
		//free( _errFile );
		//this implementation of the shell will always have the error file and output file be the same; so,
		//to avoid a double free, don't free the error file seperately.
		
	}

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
	_append = 0;
	_numberOfInputs = 0;
	_numberOfOutputs = 0;
	//printf("cleared\n");
}

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");
	
	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"temp",
		_inFile?_inFile:"temp", _errFile?_errFile:"temp",
		_background?"YES":"NO");
	printf( "\n\n" );
	
}

void
Command::execute()
{


	// Don't do anything if there are no simple commands
	//printf("NSC: %i\n", _numOfSimpleCommands);
	if ( _numOfSimpleCommands == 0 ) {
		printf("no sc\n");
		prompt();
		return;
	}

	if (strcmp(_simpleCommands[0]->_arguments[0], "about") == 0) {
		printf("\n");
		printf(ANSI_COLOR_RED BBLACK "シェル - The Ugly Shell" ANSI_COLOR_RESET "\n");
		printf(BBLACK "written by Kendall Z Bowles" ANSI_COLOR_RESET "\n");
		printf(BBLACK "for the Purdue University class CS252 - Systems Programming" ANSI_COLOR_RESET "\n");
		printf(BBLACK "during the Summer of 2017" ANSI_COLOR_RESET "\n");
		printf(BBLACK "taught by Dr. Gustavo Rodriguez-Rivera" ANSI_COLOR_RESET "\n");
		clear();
		prompt();
		return;
	} 

	if (strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0) {
		printf("\n");
		printf(ANSI_COLOR_RED BBLACK "    _  _ " ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_RED BBLACK "|  (_)|_)" ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_RED BBLACK "|__(_)| \\" ANSI_COLOR_RESET "\n");

		clear();
		_exit(1);
	} 
	// Print contents of Command data structure
	//print();

	//references: cat_grep.cc
	//slide 259
	if (_numberOfInputs > 1) {
		//printf("ERROR: More than one input file found");
		printf("Ambiguous output redirect");
		clear();
		prompt();
		return;
	}
	if (_numberOfOutputs > 1) {
		//printf("ERROR: More than one output file found");
		printf("Ambiguous output redirect");
		clear();
		prompt();
		return;
	}
	
	if (strcmp(_simpleCommands[0]->_arguments[0], "setenv") == 0) {
		int overwrite = 1;
		
		int sv = setenv(_simpleCommands[0]->_arguments[1],
							_simpleCommands[0]->_arguments[2],
							overwrite);
		if (sv < 0) {
			perror("setenv");
		}
		clear();
		prompt();
		return;
		
	} 
	
	if (strcmp(_simpleCommands[0]->_arguments[0], "unsetenv") == 0) {
		//int overwrite = 1;
		
		int usv = unsetenv(_simpleCommands[0]->_arguments[1]);
		
		if (usv < 0) {
			perror("unsetenv");
		}
		clear();
		prompt();
		return;
	} 
	
	if (strcmp(_simpleCommands[0]->_arguments[0], "cd") == 0) {
		//int overwrite = 1;
		
		int cd;
		if (_simpleCommands[0]->_numOfArguments == 1) {
			cd = chdir(getenv("HOME"));
		} else {
			cd = chdir(_simpleCommands[0]->_arguments[1]);
		}
		
		if (cd < 0) {
			perror("cd");
		}
		clear();
		prompt();
		return;
	} 
	
	if (strcmp(_simpleCommands[0]->_arguments[0], "..") == 0) {
		//int overwrite = 1;
		
		int cd = chdir("..");
		
		if (cd < 0) {
			perror("cd");
		}
		clear();
		prompt();
		return;
	} 
	
	int tempin = dup(0);
	int tempout = dup(1);
	int temperr = dup(2);
	
	int fdin = 0;
	
	if (_inFile) {
		fdin = open(_inFile, O_RDONLY);
	} else {
		fdin = dup(tempin);
	}
	
	int fderr;
	

	dup2(fderr, 1);
	close(fderr);
	
	
	int fdout = 0;
	int i = 0;
	int ret;
	for (i = 0; i < _numOfSimpleCommands; i++) {
		dup2(fdin, 0);
		close(fdin);
		//printf("starting command %i: %s\n", i, _simpleCommands[i]->_arguments[0]);
		if (i == _numOfSimpleCommands - 1) {
			// Last simple command
			//printf("last\n");
			if (_outFile) {
				if (!_append) {
					fdout = open(_outFile, O_WRONLY | O_CREAT | O_TRUNC , 0600);
				} else {
					fdout = open(_outFile, O_WRONLY | O_CREAT | O_APPEND , 0600);
				}
			} else {
				fdout = dup(tempout);
			}
		} else {
			//printf("not last\n");
			// Not last simple command
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		//printf("e\n");
		dup2(fdout, 1);
		if (_errFile) {
			dup2(fdout, 2);
		}
		close(fdout);
		//printf("ee\n");
		ret = fork();
		
		if (ret == 0) {
			
			//handle PRINTENV
			//slide 282
			
			//every reference slide I see has the simplecommands[i] in the following
			//line be _simpleCommands[0]. It won't redirect unless I have it be [i].
			//????
			
			if (strcmp(_simpleCommands[i]->_arguments[0], "printenv") == 0) {
				char **p = environ;
				while(*p != NULL) {
					printf("%s\n", *p);
					p++;
				}
				exit(1);
			}
			
			if (strcmp(_simpleCommands[i]->_arguments[0], "source") == 0) {
				FILE* file = fopen(_simpleCommands[i]->_arguments[1], "r");
				char line[256];
				while (fgets(line, sizeof(line), file)) {
					//yylex(line);
					
				}
				fclose(file);
				//printf("source\n");
				exit(1);
			}			
			if (strcmp(_simpleCommands[i]->_arguments[0], "jobs") == 0) {
				exit(1);
			}
			//close(fdin);
			execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
			perror("execvp");
			exit(1);
		} else if (ret < 0) {
			perror("fork");
			clear();
			prompt();
			return;
		} else {
			//nop
		}
		

		//printf("f\n");

		//dup2(temperr, 2);
		

		//close(tempin);
		//close(tempout);

		//close(temperr);
		//printf("g\n");


		
	}
	
	// Clear to prepare for next command
	//printf("about to clear for %i\n", i);
	
	dup2(tempin, 0);
	dup2(tempout, 1);
	dup2(temperr, 2);
	
	close(tempin);
	close(tempout);
	close(temperr);
	
	if (!_background) {
			waitpid(ret, NULL, 0);
			//prompt();
	}
	// Print new prompt
	//printf("about to prompt for %i\n", i);
	//fflush(stdin);
	//printf("prompt for %s\n", _simpleCommands[i]->_arguments[0]);
	clear();
	prompt();
}

// Shell implementation



void
Command::prompt()
{
	//very over-complicated shell prompt
	//TODO: get pwd
	
	if ( isatty(0) ) {
		numCommands++;
		if (getenv("PROMPT") != NULL) {
			printf("%s", getenv("PROMPT"));
		} else {

			if (numCommands == 1) {
	printf("\n"); printf("\n");
	printf(ANSI_COLOR_RED BBLACK "シェル～シェル～シェル～シェル～シェル～シェル～シェル～シェル～シェル" ANSI_COLOR_RESET "\n");
	printf(" --.--|   |,---.    .   .,---.|    ,   .    ,---.|   |,---.|    |      \n");
	printf("   |  |---||---     |   ||  _.|    |   |    `---.|---||--- |    |      \n");
 	printf("   |  |   ||        |   ||   ||    `---'        ||   ||    |    |      \n");
	printf("   `  `   '`---'    `---'`---'`---'  |      `---'`   '`---'`---'`---'  \n");
	printf(ANSI_COLOR_RED BBLACK "シェル～シェル～シェル～シェル～シェル～シェル～シェル～シェル～シェル" ANSI_COLOR_RESET "\n");
	printf("                       by Kendall Z Bowles\n"); printf("\n");
			}

			printf("\n");
			printf(ANSI_COLOR_BLUE BBLACK "%s" ANSI_COLOR_RESET, getenv("USER"));
			printf(ANSI_COLOR_MAGENTA BBLACK "@" ANSI_COLOR_RESET);
			char hostname[128];
			gethostname(hostname, sizeof(hostname));
			printf(ANSI_COLOR_RED BBLACK "%s        " ANSI_COLOR_RESET, hostname);

  			time_t rawtime;
  			struct tm * timeinfo;

  			time ( &rawtime );
  			timeinfo = localtime ( &rawtime );


			printf(ANSI_COLOR_YELLOW BBLACK "%.24s        " ANSI_COLOR_RESET, asctime (timeinfo)); //.24 to crop out newline char
			printf(ANSI_COLOR_GREEN BBLACK "# %i " ANSI_COLOR_RESET, numCommands);
			printf("\n");
			printf(ANSI_COLOR_CYAN BBLACK "シェル～" ANSI_COLOR_RESET " ");

		}
		
		//printf(">");
		fflush(stdout);
	} else {
		//printf("no prompt?");
	}
	
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;


extern "C" void ZombieHandler( int sig )
{
	//printf("\n      Ouch!\n");
	
	int pid;
	int status;
	
	while(1) {
		int pid = wait3(&status, WNOHANG, NULL);
		if (pid < 1) {
			return;
		}
		
		//Command::_currentCommand.clear();
		//printf("%i exited\n", pid);
		//Command::_currentCommand.prompt();
		
		//environment tests mess up if I print here
		
	}

	return;
}

extern "C" void sigIntHandler( int sig )
{
	//fprintf( stderr, "\n      Ouch!\n");
	//printf("\n      uuuu!\n");
	
	Command::_currentCommand.clear();
	printf("\n");
	Command::_currentCommand.prompt();
	return;
}





main()
{
	
	struct sigaction signalAction;
    	signalAction.sa_handler = sigIntHandler;
        sigemptyset(&signalAction.sa_mask);
        signalAction.sa_flags = SA_RESTART;
		
	int error = sigaction(SIGINT, &signalAction, NULL);
		
	if(error){
    	perror("sigaction");
    	exit(2);
    }
	
	struct sigaction signalAction2;
    	signalAction2.sa_handler = ZombieHandler;
        sigemptyset(&signalAction2.sa_mask);
        signalAction2.sa_flags = SA_RESTART;
	
	
	int error2 = sigaction(SIGCHLD, &signalAction2, NULL);
	
	if(error2){
    	    perror("sigaction");
    	    exit(2);
    }

	Command::_currentCommand.prompt();
	yyparse();
}

