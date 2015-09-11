/* $Id: Macros.h,v 1.3 2015/04/27 02:27:14 sentient Exp sentient $ 
 *
 * Author:      Dan Wilder
 * Class:       CS 2750
 * Instructor:  Professor José Cárdenas-Haro
 * Due Date:    TBA
 *
 *
 * REVISION HISTORY
 *   $Log: Macros.h,v $
 *   Revision 1.3  2015/04/27 02:27:14  sentient
 *   Increased BUFFER_LEN to 256
 *
 *   Revision 1.2  2015/04/27 01:18:28  sentient
 *   Use with v1.4.1.5. Added MAX_SHELL_ARGS
 *
 *   Revision 1.1  2015/04/25 21:44:09  sentient
 *   Initial revision
 *
 *
 */

#ifndef MACROS_H
#define MACROS_H

// Shell Input Buffer
#define BUFFER_LEN 256
#define MAX_SHELL_ARGS 16

// Searching for command:
#define FULL_PATH_LEN 256
#define PATH_LEN 223
#define COMMAND_LEN 32

// Probing command macro values
#define EXECUTE_YES 1
#define REG_FILE 2

#endif
