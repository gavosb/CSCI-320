/*
 * part_3.c
 *
 * Takes its initial arguments and prints them on the standard output,
 * each argument getting its own line (including the name of the program).
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

/*
 * Definitions
 */
#define TRUE  1
#define FALSE 0

int main(int argc, char **argv){

    int i;
    for (i = 0; i < argc; i++) {
	    printf("%s\n", argv[i]);
    }
    
    return 0;
	
}
