/*
 * part_5.c
 *
 * Executes another program given by its argument, and then closes the program.
 *
 * Operations:
 * None
 *
 * Data Structures:
 * None
 *
 * by Gavin Osborn 09/09/2023
 */

/*
 * Imports
 */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int main(int argc, char **argv){

    char *newArgv[argc]; // argc starts at 1 without arguments
    for (int i = 0; i < argc; i++) { // start at 0 and copy for argc-1
        newArgv[i] = argv[i+1]; // argv starts with the program name, so this is fine
    }
    newArgv[argc - 1] = NULL; // argv has to be null terminated
    
    if (execvp(argv[1], newArgv) == -1) { // try: replace current program with new program without forking
        perror("execvp failed to open program in main()");
        return 1; // error
    }
    
    return 0; // should never get here
	
}

