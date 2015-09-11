/* $Id: Command.c,v 1.3 2015/04/26 05:54:49 sentient Exp sentient $ 
 *
 * Author:      Dan Wilder
 * Class:       CS 2750
 * Instructor:  Professor José Cárdenas-Haro
 * Due Date:    TBA
 *
 *
 * REVISION HISTORY
 *   $Log: Command.c,v $
 *   Revision 1.3  2015/04/26 05:54:49  sentient
 *   Slight modification (possibly). For use with mysh.c v1.4.1.4
 *
 *   Revision 1.2  2015/04/25 21:45:50  sentient
 *   For use wtih mysh.c v1.4.1.2
 *   Changed canExecuteCommand to more general probe_Command
 *
 *   Revision 1.1  2015/04/25 19:35:47  sentient
 *   Initial revision
 *
 *
 *
 */

#include "../include/Command.h"
#include "../include/Macros.h"
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*****************************************************************************
 *  newCommand                                                               *
 *****************************************************************************
 * Creates a new Command. Given a name and a list of directory paths 
 * separated by a ':', function looks for the command in each of the paths
 * until found or not found. If found, path attribute will be set, otherwise
 * it will be NULL. If found, the owner attribute will also be set which
 * corresponds to owner of the found file. If not found, owner attribute
 * will be NULL. *NOTE* Path length limitation!
 */

Command newCommand(char *name, char *pathList) {

  char *tempPathList;
  char *searchDir;
  char fullPath[FULL_PATH_LEN];

  struct stat *statResults;
  statResults = (struct stat *)malloc(sizeof(struct stat));

  // Set memory aside for self 
  Command self = (Command)malloc(sizeof(struct Command_Struct));

  // Set name
  self->name = (char *)malloc(strlen(name));
  strcpy(self->name, name);
  
  // Set base case (i.e. not found)
  self->path = NULL;
  self->stats = NULL;

  // Set methods
  self->probe = &probe_Command;
  self->destruct = &destroy_Command;
  
  // Prevent strtok from modifying original
  tempPathList = (char *) malloc(strlen(pathList));
  strcpy(tempPathList, pathList);

  // Search for command 
  searchDir = strtok(tempPathList, ":");

  while( searchDir != NULL ) {
 
      // Construct full path as: searchDir/userCommand
      strncpy(fullPath, searchDir, PATH_LEN); 
      strncat(fullPath, "/", 1);
      strncat(fullPath, self->name, COMMAND_LEN);

      // Command doesn't exist in search directory -> Update search directory
      if ( stat(fullPath, statResults) == -1 ) 
        searchDir = strtok(NULL, ":");
          
      // Command exists in search directory
      else {

        // Set path
        self->path = (char *)malloc(strlen(fullPath));
        strcpy(self->path, fullPath);

        // Set stats
        self->stats = statResults;

        // Stop looking for command; Exit while loop
        break;
      }
    }

  free(tempPathList);
  return self;
}

/*****************************************************************************
 *  probe_Command                                                            *
 *****************************************************************************
 * Result contructed as sum of macro values. Each of which represents a 
 * certain condition. Each macro value is some power of 2 which follows then 
 * that it is represented as a binary string with a single 1 in some position.
 * Thus, adding them together will result in a binary string with 1s ONLY
 * in positions that were present in one of the macro values. 
 * 
 * Outside code can use probe_Command's results with '& macro_value' to test
 * if the condition represented by macro_value was present. 
 */

int probe_Command(const void *selfObj) {
 
  Command command = (Command)selfObj;
  uid_t uid = getuid();

  int results = 0;

  // Make sure that command has been found before checking other conditions
  if ( command->path == NULL )
    return results;

  // Check if command is 'regular' file
  if ( S_ISREG(command->stats->st_mode) )
    results += REG_FILE;

  // User is OWNER
  if (uid == command->stats->st_uid) {
    results += (command->stats->st_mode & S_IXUSR) ? EXECUTE_YES : 0;
  }
  
  // User is OTHER
  else {
    results += (command->stats->st_mode & S_IXOTH) ? EXECUTE_YES : 0;
  }

  return results;
}

/*****************************************************************************
 *  destroy_Command                                                          *
 *****************************************************************************
 * Destructor. Dynamic memory allocation requires cleanup.
 */

void destroy_Command(const void *selfObj) {
 
  Command self = (Command)selfObj;

  free(self->name);
  free(self->path);
  free(self->stats);
  free(self);
}
