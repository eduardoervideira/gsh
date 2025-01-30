#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "terminal.h"
#include "input.h"

int main(int argc, char **argv){
    //signal(SIGINT, SIG_IGN);
    struct termios old_termios;
    set_raw_mode(&old_termios);

    if(argc > 1){
        fprintf(stderr, "gsh can't execute scripts (yet)\n");
        exit(EXIT_FAILURE);
    } else {
        bool running = true;
        char *prompt = "gsh> ";
        char input[MAX_INPUT_LENGTH];
        size_t cursor_pos = 0;
        size_t input_length = 0;

        while (running) {
            get_input(input, &cursor_pos, &input_length); // check error code?

            if (!strcmp(input, "exit")) {
                running = false;
            } else {
                printf("\n");
                printf("< %s\n", input);
                memset(input, 0, MAX_INPUT_LENGTH);
                cursor_pos = 0;
                input_length = 0;
            }
        }
    }

    reset_terminal(&old_termios);

    return 0;
}