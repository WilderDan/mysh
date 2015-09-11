#mysh: A simple shell program

*** Commented out extracting PS1 in mysh.c ***

Dan Wilder
CS 2750
Project #4

======================
Overview
======================

SYNOPSIS
    <EXECUTABLE>
    
    Default name is mysh
 

DESCRIPTION
    Minimal shell program (mysh) for Unix/Unix-like. On startup extracts
    value of environment variables, MYPATH and PS1. If MYPATH isn't
    set, then mysh uses value of PATH. Shell takes input from user
    and parses into tokens representing words. The first word corresponds to 
    the command to search. Subsequent words are passed as arguments to the 
    command. Some error checking included: command exists before trying to 
    execute, user has permission to run command, command is regular file.


BUILT-IN COMMANDS
    
    exit    -    close shell program
 

KEYBOARD INTERACTIVITY

    Ctrl-C    -    Send interrupt signal. Causes termination of currently
                   executing command.

    Ctrl-D    -    Exit shell.


LIMITATIONS

    Finite limits on:
        ~ Macros.h defines these limits

        1) Raw user input (BUFFER_LEN)
        2) Max number space-delimited arguments read (MAX_SHELL_ARGS)
        3) Length of command path to search


======================
INSTRUCTIONS
======================

  [BUILD PROJECT] 

      0) ***OPTIONAL*** 
         Specify EXECUTABLE name in Makefile. You can also include pathname to
         install in different directory. Default name is 'mysh' in project's 
         root directory. 

      1) Run command 'make' from project's root directory.
               
  [CLEANUP] 
 
      1) Run command 'make clean' from project's root directory.
           ~ executable and object files will be removed  
