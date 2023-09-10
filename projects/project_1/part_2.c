/*
 * part_2.c
 *
 * Reads from the standard input one line at a time, and writes the line it just read to the standard output 
 * Terminate when the end-of-file is reached without writing anything.
 *
 * gets() and puts() were removed from the C standard and replaced with fgets().
 * Due to the confusion this caused, this code is largely from ChatGPT.
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

	char line_arr[50]; // max size for string/line is 50

    while (fgets(line_arr, sizeof(line_arr), stdin) != NULL) { // read from stdin into line_arr
        
        fputs(line_arr, stdout); // Write to standard output
    }
    
    return 0;
	
}
