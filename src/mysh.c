/*
 *
 * Author:      Dan Wilder
 * Class:       CS 2750
 * Instructor:  Professor José Cárdenas-Haro
 * Due Date:    TBA
 *
 *
 * REVISION HISTORY
 *
 *   FROM using RCS
 *
 *   $Log: mysh.c,v $
 *   Revision 1.4.1.8  2015/04/29 03:22:22  sentient
 *   More corrections to improper memory freeing
 *
 *   Revision 1.4.1.7  2015/04/29 02:48:15  sentient
 *   Bug fix in freeing memory... forgot to increment i!!!!
 *
 *   Revision 1.4.1.6  2015/04/27 02:00:59  sentient
 *   Slight change. Send SIGTERM to child process instead SIGINT
 *   when Ctrl-C (SIGINT) detected by parent.
 *
 *   Revision 1.4.1.5  2015/04/27 00:56:35  sentient
 *   Final revision. Added myargv to tokenize command line input.
 *
 *   Revision 1.4.1.4  2015/04/26 05:51:26  sentient
 *   Added signal handler for ctrl-c to kill currently
 *   executing child process. ctrl-d from shell will 
 *   kill shell. Slight limitation: ctrl-d must be issued 
 *   with empty input preceeding.
 *
 *   Revision 1.4.1.3  2015/04/26 01:00:51  sentient
 *   Place holder for signal handling. Empty input is no longer accepted
 *   from user.
 *
 *   Revision 1.4.1.2  2015/04/25 21:39:37  sentient
 *   Can check if command is regular file
 *
 *   Revision 1.4.1.1  2015/04/25 19:41:01  sentient
 *   New line of development by creating Command_Struct and typedef
 *   Command to be a pointer to such a structure. The idea is to create
 *   (hopefully) more scalable code. Additionally, shell now checks
 *   if user has permission to execute a command before attempting to
 *   execute it.
 *
 *   Revision 1.4  2015/04/24 04:56:26  sentient
 *   Program checks if environment variable PS1 has been set. 
 *   Basic idea of checking is user has permission to execute the command.
 *
 *   Revision 1.3  2015/04/24 02:53:59  sentient
 *   Cleaned code up a bit more. Added built-in exit command to terminate 
 *   shell.
 *
 *   Revision 1.2  2015/04/23 23:35:11  sentient
 *   Basic functioning of 1 word input (the command) present. Works with
 *   Default Path. Uses default prompt of >>>
 *
 *   Revision 1.1  2015/04/23 22:38:28  sentient
 *   Initial revision
 *
 */

#include "../include/Command.h"
#include "../include/Macros.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void interruptHandler(int);
void splitWords(const char *source); 

pid_t pid; // Use wtih interruptHandler
char *myargv[MAX_SHELL_ARGS];

/*****************************************************************************
 *  main                                                                     *
 *****************************************************************************/

int main(int argc, char **argv) {
   
  char *prompt;
  char *envPath;
  char *inputString;
  char inputBuffer[BUFFER_LEN];
  
  int i;
  int results;
  int *returnStatus = NULL;

  Command command;
  
  // Signal Handler
  signal(SIGINT, interruptHandler); 

  // Extract environment variable MYPATH; If not set, use default PATH
  envPath = getenv("MYPATH");
  if (envPath == NULL)
    envPath = getenv("PATH");

  // Extract environment variable PS1; If not set, use default >>> 
  //prompt = getenv("PS1");
  //if (prompt == NULL)
    prompt = ">>> ";
   
  // Shell Loop
  while (1) {
    
    // Null myargv out
    for (i=0; i<MAX_SHELL_ARGS; i++) {
      myargv[i] = NULL;
    }

    // Prompt
    printf("%s", prompt);

    // Retrieve nonempty input from user; strtok removes trailing new line
    do {
      if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
        // User hit ctrl-d on empty input

        printf("exiting...\n");
        exit(EXIT_SUCCESS);
      }

      inputString = ( strlen(inputBuffer) > 1) ? strtok(inputBuffer, "\n") : "";
    } while ( strcmp("", inputString) == 0 );

    // Store each word of inputString to index of myargv; 
     splitWords(inputString); // words split into myargv[index]

    if (myargv[0] == NULL)
      command = newCommand("", envPath);
    else 
      command = newCommand(myargv[0], envPath);
    
    results = command->probe(command);

    /*** 5 Mutually exclusive blocks ***/

    // 1) Built-in exit command
    if ( strcmp("exit", command->name) == 0 )
      exit(EXIT_SUCCESS);

    // 2) Command not found
    else if (command->path == NULL) {
      printf("  [%s] '%s': command not found\n", argv[0], command->name);
    }

    // 3) Command is not a regular file
    else if ( !(results & REG_FILE) ) {
      printf("  [%s] '%s': Cannot execute: ", argv[0], command->name); 
      printf("Not a regular file\n");
    }

    // 4) User lacks execute permission
    else if ( !(results & EXECUTE_YES ) ) {
      printf("  [%s] '%s': Cannot execute: ", argv[0], command->name); 
      printf("Permission Denied\n");
    }

    // 5) Command was found and can be executed
    else {

      // Fork the process
      pid = fork();

      // Child (command) Process: Execute specified command
      if (pid == 0) 
        //execl(command->path, command->name, NULL);
        execv(command->path, myargv); 
      
      // Parent (mysh) Process:  Wait until child (command) finishes
      else  
        waitpid(pid, returnStatus, 0);    
    }
    
    command->destruct(command);
 
    // Free any existing memory
    for (i=0; i<MAX_SHELL_ARGS; i++) {
      free(myargv[i]);
      myargv[i] = NULL;
    }
  }

  return 0;
}

/*****************************************************************************
 *  splitWords                                                               *
 *****************************************************************************/
/* Stores each word of a source string into a dest index. 
 *
 * Limitation: Stores only up to the value of MAX_SHELL_ARGS words
 *
 */

void splitWords(const char *source) {

  char *token;
  int i;

  // Prevent modification to original
  char *duplicate = (char *)malloc(strlen(source));
  strcpy(duplicate, source);
  
  // Get first word
  token = strtok(duplicate, " ");

  if (token != NULL) { 
    myargv[0] = (char *)malloc(strlen(token));
    strcpy(myargv[0], token);
  }
  else
    myargv[0] = NULL;
  
  // Subsequent Words
  i = 0;
  while ( myargv[i] != NULL ) {
    ++i;
    
    // Only store up to MAX_SHELL_ARGS
    if (i == MAX_SHELL_ARGS) 
      break;

    // Get next word
    token = strtok(NULL, " ");
   
    if (token != NULL) {
      myargv[i] = (char *)malloc(strlen(token));
      strcpy(myargv[i], token);
    }
    else
      myargv[i] = NULL;
  } 

  free (duplicate);
}

/*****************************************************************************
 *  interruptHandler                                                         *
 *****************************************************************************/
/* Kill currently executing command.
 */
void interruptHandler(int sigint) {

  if (kill(pid, 0)) {
    kill(pid, SIGTERM);
  }
}
