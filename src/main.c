#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "terminal.h"
#include "input.h"
#include "tokenizer.h"
#include "executor.h"

int main(int argc, char **argv){
    //signal(SIGINT, SIG_IGN);
    struct termios old_termios;
    set_raw_mode(&old_termios);

    if(argc > 1){
        fprintf(stderr, "gsh can't execute scripts (yet)\n");
        exit(EXIT_FAILURE);
    } else {
        printf("\033[2J\033[H");
        bool running = true;
        char *prompt = "gsh> ";
        char input[MAX_INPUT_LENGTH];
        size_t cursor_pos = 0;
        size_t input_length = 0;

        while (running) {
            // clean all the terminal before
            get_input(input, &cursor_pos, &input_length); // check error code?
            printf("\n");

            if(!strcmp(input, "exit")){
                running = false;
            } else {
                Token *tokens = NULL;
                int num_tokens = tokenize(input, &tokens);

                if(num_tokens > 0){
                    execute(tokens, num_tokens);

                    /*for (int i = 0; i < num_tokens; i++){
                        printf("  [%s] -> ", tokens[i].value);
                        print_token(tokens[i].type);
                        printf("\n");
                    }*/
                }

                for(int i = 0; i < num_tokens; i++)
                    free(tokens[i].value);

                free(tokens);
            }
        }
    }

    reset_terminal(&old_termios);
    return 0;
}