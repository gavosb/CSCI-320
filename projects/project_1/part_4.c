/*
 * part_4.c
 *
 * Reads the standard input a line at a time, identifies the tokens 
 * (sequences of characters that do not include blanks or tabs), and prints the tokens on the standard output,
 * each one with its own line.
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
# include <string.h>

int main(int argc, char **argv){

    char line_arr[50]; // max size for string/line is 50
    
    while (fgets(line_arr, sizeof(line_arr), stdin) != NULL) { // read from stdin into line_arr
        print_tokens(line_arr); // print tokens
        
    }
    
    return 0;
	
}

/*
 * print_tokens()
 * Tokenizes a string into separate strings and then prints them.
 * Ideally this would be split into two, storing the strings in a data structure. I'm too lazy.
 *
 * Parameters:
 * - char* line_arr: pointer to line string
 *
 */
void print_tokens(char *line_arr){

    char *whitespace = " \t\f\r\v\n";
    
    /* note: this basically just keeps updating the pointer to be the next 'token' from the original line array */
    char *token = strtok(line_arr, whitespace);       // get the first token 
    
    while (token != NULL) {
    
        printf("%s\n", token);
        token = strtok(NULL, whitespace);     // gets next token
        
    }
}
