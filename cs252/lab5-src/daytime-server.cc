
const char * usage =
"                                                               \n"
"daytime-server:                                                \n"
"                                                               \n"
"Simple server program that shows how to use socket calls       \n"
"in the server side.                                            \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   daytime-server <mode> <port>                                \n"
"                                                               \n"


"Where 1024 < port < 65536,             			\n"
"and mode = -f, -t, -p, or -i.             			\n"

"                                                               \n"
"-f :  Create a new process for each request     		\n"
"-t :  Create a new thread for each request     			\n"
"-p :  Use a pool of threads     					\n"
"-i :  Create an iterative server     					\n"
"                                                               \n"
"If port is not given, the default port is 18750.                \n"
"If mode is not given, the default mode is -i.                \n"
"                                                               \n"
"In another window type:                                       \n"
"                                                               \n"
"   telnet <host> <mode> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where daytime-server  \n"
"is running. <port> is the port number you used when you run   \n"
"daytime-server. <mode> is the desired style of thread          \n"
"concurrency.          											\n"
"                                                               \n"
"Then type your name and return. You will get a greeting and   \n"
"the time of the day.                                          \n"
"                                                               \n";


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

extern "C" void ZombieHandler(int sig);
extern "C" void sigIntHandler( int sig );

int QueueLength = 5;

// Processes time request
void processTimeRequest( int socket );

bool isNumber(char number[]);
void * processAndClose( int fd );
void * loopthread(void * masterSocket);

//declare these here so everything can use them
pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;


main( int argc, char ** argv )
{
  // Print usage if not enough arguments
  if ( argc < 1 || argc > 3) {
    fprintf( stderr, "%s", usage );
    exit( -1 );
  }
  
  // Get the port from the arguments

  char * mode;



  if (argc > 1 && !isNumber(argv[1])) {
	mode =  argv[1];
  } else {
	mode = (char *) "-i";
  }
  int cmode;

  if (strcmp(mode, "-f") == 0) {
	printf("mode: Create a new process for each request\n");
	cmode = 0;
  } else if (strcmp(mode, "-t") == 0) {
	printf("mode: Create a new thread for each request\n");
	cmode = 1;
  } else if (strcmp(mode, "-p") == 0) {
	printf("mode: Create a pool of threads\n");
	cmode = 2;
  } else if (strcmp(mode, "-i") == 0) {
	printf("mode: Create an iterative server\n");
	cmode = 3;
  } else {
	fprintf( stderr, "%s", usage );
	exit( -1 );
  }
  
  int port;

  if (argc == 1 || (argc == 2 && !isNumber(argv[1]))) {
	port = 18750;
  } else if (argc == 2 && isNumber(argv[1])){
	port = atoi( argv[1] );
  } else if (argc == 3 && isNumber(argv[2])){
	port = atoi( argv[2] );
  } else {
    fprintf( stderr, "%s", usage );
    exit( -1 );
  }

  if (port <= 1024 || port >= 65536) {
	fprintf( stderr, "%s", usage );
    exit( -1 );
  }
  
  printf("using port: %i\n", port);
  
  //zombie handling, shamelessly copied from shell...
	struct sigaction signalAction;
    	signalAction.sa_handler = sigIntHandler;
        sigemptyset(&signalAction.sa_mask);
        signalAction.sa_flags = SA_RESTART;
		
	int error3 = sigaction(SIGINT, &signalAction, NULL);
		
	if(error3){
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
	
  // Set the IP address and port for this server
  struct sockaddr_in serverIPAddress; 
  memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
  serverIPAddress.sin_family = AF_INET;
  serverIPAddress.sin_addr.s_addr = INADDR_ANY;
  serverIPAddress.sin_port = htons((u_short) port);
  
  // Allocate a socket
  int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
  if ( masterSocket < 0) {
    perror("socket");
    exit( -1 );
  }

  // Set socket options to reuse port. Otherwise we will
  // have to wait about 2 minutes before reusing the sae port number
  int optval = 1; 
  int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
		       (char *) &optval, sizeof( int ) );
   
  // Bind the socket to the IP address and port
  int error = bind( masterSocket,
		    (struct sockaddr *)&serverIPAddress,
		    sizeof(serverIPAddress) );
  if ( error ) {
    perror("bind");
    exit( -1 );
  }
  
  // Put socket in listening mode and set the 
  // size of the queue of unprocessed connections
  error = listen( masterSocket, QueueLength);
  if ( error ) {
    perror("listen");
    exit( -1 );
  }
 
  
  if (cmode == 3) { //iterative
	  while ( 1 ) {

	    // Accept incoming connections
	    struct sockaddr_in clientIPAddress;
	    int alen = sizeof( clientIPAddress );
	    int slaveSocket = accept( masterSocket,
				      (struct sockaddr *)&clientIPAddress,
				      (socklen_t*)&alen);

	    if ( slaveSocket < 0 ) {
	      perror( "accept" );
	      exit( -1 );
	    }

	    // Process request.
	    processAndClose( slaveSocket );
	    //processAndClose( slaveSocket );
	  }
  } else if (cmode == 0) { //fork
	  while ( 1 ) {
        //printf("starting fork loop\n");
	    // Accept incoming connections
	    struct sockaddr_in clientIPAddress;
	    int alen = sizeof( clientIPAddress );
	    int slaveSocket = accept( masterSocket,
				      (struct sockaddr *)&clientIPAddress,
				      (socklen_t*)&alen);

	    if ( slaveSocket < 0 ) {
	      perror( "accept" );
	      exit( -1 );
	    }
	    int ret = fork();
	    if (ret == 0) {
			// Process request.
			//printf("child\n");
			processTimeRequest( slaveSocket );
			exit( 1 );
        } else if (ret < 0) {
			//perror("fork");
			exit( 1 );
	    } else {
			//waitpid(ret, 0, 0);
			//printf("done waiting\n");
			close(slaveSocket);
			
			//IDK why this makes it work... but it does
		}
	  }
  } else if (cmode == 1) { //thread
	  while ( 1 ) {

	    // Accept incoming connections
	    struct sockaddr_in sockInfo;
	    int alen = sizeof( sockInfo );
	    int slaveSocket = accept( masterSocket,
				      (struct sockaddr *)&sockInfo,
				      (socklen_t*)&alen);

	    if ( slaveSocket < 0 ) {
	      perror( "accept" );
	      exit( -1 );
	    }
		//printf("got this far...\n");
	    pthread_t thread;
	    pthread_attr_t attr; 
	    pthread_attr_init(&attr); 
	    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //took a long time to find this
		//printf("and here...\n");
		
		void * slaveSocketVoid = &slaveSocket; //this technique stops compiler warnings
	    pthread_create(&thread, &attr, (void * (*)(void *)) processAndClose, slaveSocketVoid);
		
		//printf("and good!\n");
	}
  } else if (cmode = 2) { //pool of threads
		//printf("got this far...\n");
		
		int numThreads = 5;
		
		pthread_mutexattr_init(&mutexattr);
        pthread_mutex_init(&mutex, &mutexattr);
		
		pthread_t thread[numThreads]; 
		pthread_attr_t attr;
		pthread_attr_init(&attr); 
		//printf("and here...\n");
		for (int i = 0; i < numThreads; i++) { 
			//printf("loop: %i\n", i);
			void * masterSocketVoid = &masterSocket;
			pthread_create(&thread[i], &attr, (void * (*)(void *)) loopthread, masterSocketVoid);
			//void * masterSocketVoid = &masterSocket;
			//loopthread(masterSocketVoid);
		}
		//printf("and good!\n");
		
		pthread_join(thread[0], NULL);
  }
}

void * loopthread(void * masterSocketVoid) {
	//printf("beginning loopthread\n");
	int masterSocket = *((int *)masterSocketVoid);
	//printf("masterSocket initialized: %i\n", masterSocket);
	while (1) {
		
		pthread_mutex_lock(&mutex);
		
		struct sockaddr_in sockInfo;
		int alen = sizeof(sockInfo); 
		int slaveSocket = accept(masterSocket, (struct sockaddr *)&sockInfo, (socklen_t *)&alen); 
		
		pthread_mutex_unlock(&mutex);
		
		if (slaveSocket >= 0) { 
			processAndClose(slaveSocket); 
		} 
		
	} 
}


void * processAndClose(int slaveSocket) {
	//printf("processing: %i\n", slaveSocket);
	processTimeRequest( slaveSocket );
	//printf("closing: %i\n", slaveSocket);
	close( slaveSocket );
	//printf("closed: %i\n", slaveSocket);
}

void
processTimeRequest( int fd )
{
  // Buffer used to store the name received from the client
  const int MaxName = 1024;
  char name[ MaxName + 1 ];
  int nameLength = 0;
  int n;

  // Send prompt
  const char * prompt = "\nType your name:";
  write( fd, prompt, strlen( prompt ) );

  // Currently character read
  unsigned char newChar;

  // Last character read
  unsigned char lastChar = 0;

  //
  // The client should send <name><cr><lf>
  // Read the name of the client character by character until a
  // <CR><LF> is found.
  //
    
  while ( nameLength < MaxName &&
	  ( n = read( fd, &newChar, sizeof(newChar) ) ) > 0 ) {

    if ( lastChar == '\015' && newChar == '\012' ) {
      // Discard previous <CR> from name
      nameLength--;
      break;
    }

    name[ nameLength ] = newChar;
    nameLength++;

    lastChar = newChar;
  }

  // Add null character at the end of the string
  name[ nameLength ] = 0;

  printf( "name=%s\n", name );

  // Get time of day
  time_t now;
  time(&now);
  char	*timeString = ctime(&now);

  // Send name and greetings
  const char * hi = "\nHi ";
  const char * timeIs = " the time is:\n";
  write( fd, hi, strlen( hi ) );
  write( fd, name, strlen( name ) );
  write( fd, timeIs, strlen( timeIs ) );
  
  // Send the time of day 
  write(fd, timeString, strlen(timeString));

  // Send last newline
  const char * newline="\n";
  write(fd, newline, strlen(newline));
  //printf("finished processing: %i\n", fd);
  
}

bool isNumber(char number[])
{
    int i = 0;

    //checking for negative numbers
    if (number[0] == '-') {
        i = 1;
    }
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i])) {
            return false;
        }
    }
    return true;
}

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
	//printf("exiting...");
	exit(1);
	return;
}