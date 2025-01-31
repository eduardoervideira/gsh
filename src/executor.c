#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "executor.h"
#include "tokenizer.h"

int handle_builtin(Token *tokens, int num_tokens) {
    if (strcmp(tokens[0].value, "cd") == 0) {
        if (num_tokens < 2) {
            fprintf(stderr, "cd: missing argument\n");
            return 1;
        }
        if (chdir(tokens[1].value) != 0) {
            perror("cd");
            return 1;
        }
        return 1;
    }
    return 0;
}

Command *parse_commands(Token *tokens, int num_tokens, int *num_commands) {
    Command *commands = NULL;
    *num_commands = 0;
    
    Command current_cmd = {NULL, 0};
    
    for (int i = 0; i < num_tokens; i++) {
        if (tokens[i].type == TOKEN_PIPE) {
            if (current_cmd.num_tokens > 0) {
                commands = realloc(commands, (*num_commands + 1) * sizeof(Command));
                commands[*num_commands] = current_cmd;
                (*num_commands)++;
                
                current_cmd.tokens = NULL;
                current_cmd.num_tokens = 0;
            }
        } else {
            if (current_cmd.tokens == NULL) {
                current_cmd.tokens = &tokens[i];
            }
            current_cmd.num_tokens++;
        }
    }
    
    if (current_cmd.num_tokens > 0) {
        commands = realloc(commands, (*num_commands + 1) * sizeof(Command));
        commands[*num_commands] = current_cmd;
        (*num_commands)++;
    }
    
    return commands;
}

int execute(Token *tokens, int num_tokens){
    if(handle_builtin(tokens, num_tokens)){
        return 0;
    }

    int num_commands;
    Command *commands = parse_commands(tokens, num_tokens, &num_commands);
    
    if(num_commands == 0){
        free(commands);
        return 0;
    }

    // TODO: todo
    
    return 0;
}