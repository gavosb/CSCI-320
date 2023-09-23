/*
 * part_6.c
 *
 * A mini-shell, that is, it waits for input and runs a command with arguments,
 * and when that process finishes, it returns back to the original process; this continues
 * until EOF. 
 *
 * Operations:
 * None
 *
 * Data Structures:
 * None
 *
 * by Gavin Osborn 09/10/2023
 */

/*
 * Imports
 */
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>

/*
 * Definitions
 */
# define MAX 256

void read_input(char *line_arr, char **token_arr);

/*
 * main()
 *
 * Parameters:
 * - int argc: count of arguments, starts at 1
 * - int** argv: char array of arguments, first index is program name
 *
 * Return:
 * - 0: Success
 * - 1: Error
 *
 */
int main(int argc, char **argv){
    
    char line_arr[MAX]; // maximum input line size
    char *token_arr[MAX];
    
    printf("MiniSHELL$ ");
    while (fgets(line_arr, sizeof(line_arr), stdin) != NULL) { // read from stdin into line_arr
    
        printf("MiniSHELL$ ");
        
        read_input(line_arr, token_arr);
        
        if (tokens[0] == NULL) {
            continue; // don't bother running the rest if no command
        }
        
        int pid = fork(); // fork to execute program and be able to return to parent (shell)
        
        if (pid == 0) { // child
        
            if (execvp(token_arr[0], token_arr) == -1) {
                perror("execvp failed to open program in main()");
                exit(1); // error
            }
            exit(0);
            
        } else { // parent
        
            int status;
            if (wait(&status) == -1) {
                perror("wait error in main()");
                exit(1); // error
            }
            
        }
        
    } 
    
    return 0; // EOF
	
}

/*
 * read_input()
 * 
 * Tokenizes input line and modifies tokens parameter.
 *
 * Parameters:
 * - char* line_arr: original input line
 * - char** token_arr: double ptr to array where elements point to strings
 *
 * Return:
 * - modifies token_arr
 */
void read_input(char *line_arr, char **token_arr){

    int newArgc = 0;
    char *whitespace = " \t\f\r\v\n";
    char *currentToken = strtok(line_arr, whitespace);
    
    while (currentToken != NULL) { // for length of token_arr
        token_arr[newArgc] = strdup(currentToken); // duplicate each token again because strings are just pointers
        currentToken = strtok(NULL, whitespace);
        newArgc++;
    }
    
    token_arr[newArgc] = NULL;

}
