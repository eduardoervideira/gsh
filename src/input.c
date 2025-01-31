#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "terminal.h"
#include "tokenizer.h"

#define MAX_INPUT_LENGTH 1024

const char *prompt = "gsh> ";

int get_input(char *input, size_t *cursor_pos, size_t *input_length){
    int ch;
    printf("%s", prompt);
    printf("\033[5 q");

    memset(input, 0, MAX_INPUT_LENGTH);
    *cursor_pos = 0;
    *input_length = 0;

    while(1){
        ch = getchar();
        if(ch == 10){ // enter
            input[*input_length] = '\0';
            break;
        } else if(ch == 127 || ch == 8){ // backspace
            if(*cursor_pos > 0){
                memmove(input + *cursor_pos - 1, input + *cursor_pos, *input_length - *cursor_pos + 1);
                (*input_length)--;
                (*cursor_pos)--;

                printf("\r%s%s\033[K", prompt, input); // \r returns to beginning of line, \033[K clears to the end
                printf("\033[%dG", *cursor_pos + strlen(prompt) + 1);
            }
        } else if(ch == 27){
            if(getchar() == '['){
                ch = getchar();
                if(ch == 'A'){ // Up
                    // Handle up arrow (history)
                } else if(ch == 'B'){ // Down
                    // Handle down arrow (history)
                } else if(ch == 'C'){ // Right
                    if(*cursor_pos < *input_length){
                        (*cursor_pos)++;
                        printf("\033[1C");
                    }
                } else if(ch == 'D'){ // Left
                    if(*cursor_pos > 0){
                        (*cursor_pos)--;
                        printf("\033[1D");
                    }
                }
            }
        } else if(isprint(ch)){
            if(*cursor_pos < *input_length){
                for(size_t i = *input_length; i > *cursor_pos; i--){
                    input[i] = input[i - 1];
                }
            }
            input[*cursor_pos] = ch;
            (*cursor_pos)++;
            (*input_length)++;
            putchar(ch);

            for(size_t i = *cursor_pos; i < *input_length; i++){
                putchar(input[i]);
            }

            for(size_t i = *cursor_pos; i < *input_length; i++){
                printf("\b");
            }
        }
    }
    return 0;
}