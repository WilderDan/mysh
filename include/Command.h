/* $Id: Command.h,v 1.3 2015/04/25 21:45:22 sentient Exp sentient $ 
 *
 * Author:      Dan Wilder
 * Class:       CS 2750
 * Instructor:  Professor José Cárdenas-Haro
 * Due Date:    TBA
 *
 *
 * REVISION HISTORY
 *   $Log: Command.h,v $
 *   Revision 1.3  2015/04/25 21:45:22  sentient
 *   For use with mysh.c v1.4.1.2
 *
 *   Revision 1.2  2015/04/25 19:36:48  sentient
 *   Slight organizational change
 *
 *   Revision 1.1  2015/04/25 05:25:59  sentient
 *   Initial revision
 *
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <sys/types.h>

typedef struct Command_Struct* Command;

struct Command_Struct
{
  char *name;
  char *path;
  struct stat *stats;
  
  int (*probe)(const void *self);
  void (*destruct)(const void *self);
};

int probe_Command(const void *self);
void destroy_Command(const void *self);

Command newCommand(char *name, char *pathList); 

#endif 
