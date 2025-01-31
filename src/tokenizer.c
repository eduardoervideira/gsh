#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include "tokenizer.h"

#define log(fmt, ...) printf("\n[tokenizer]: " fmt, ##__VA_ARGS__)

char *expand_variable(const char *token){
    //printf("expand_variable: %s\n", token);
    if(!token || !*token) return strdup("");

    if(token[0] == '$'){
        size_t len = strlen(token);
        if(len == 1){
            //fprintf(stderr, "error: missing variable name\n");
            //return strdup("");
            return strdup("$");
        }

        size_t i = 0;
        size_t start = 1;
        size_t end = len;
        if(token[1] == '{'){
            start++;
            end--;
        }

        // TODO: need to expand variables inside the variable

        char *var = strndup(token + start, end - start);
        if(!var) return strdup("");

        const char *value = getenv(var);
        free(var);
        return strdup(value ? value : "");
    } else if(token[0] == '{'){
        size_t len = strlen(token);
        if(token[len - 1] != '}'){
            fprintf(stderr, "error: unterminated variable\n");
            return strdup("");
        }

        if(len == 2)
            return strdup("");

        char *var = strndup(token + 1, len - 2);
        if(!var) return strdup("");

        const char *value = getenv(var);
        free(var);
        return strdup(value ? value : "");
    }

    
    return strdup(token);
}

int tokenize(char *input, Token **tokens){
    if(input == NULL || tokens == NULL){
        return 0;
    }

    int num_tokens = 0;
    int capacity = 20;
    *tokens = malloc(capacity * sizeof(Token));

    if(*tokens == NULL){
        fprintf(stderr, "malloc failed %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    bool expect_command = true;
    size_t i = 0;
    while(input[i] != '\0'){
        if(isspace(input[i])){
            i++;
            continue;
        }

        if(input[i] == '\'' || input[i] == '"'){
            char quote = input[i];
            int start = i;
            i++;

            while(input[i] != quote && input[i] != '\0'){
                if(input[i] == '\\' && input[i + 1] == quote){
                    i += 2;
                    continue;
                }
                i++;
            }

            (*tokens)[num_tokens].type = TOKEN_ARGUMENT;
            (*tokens)[num_tokens].value = strndup(input + start + 1, i - start - 1);
            num_tokens++;
            i++;
            continue;
        }

        char *expanded_value = NULL;
        size_t total_len = 0;
        while(input[i] == '$'){
            int start = i;
            i++;

            char *var_name = NULL;

            if(input[i] == '{'){
                int var_start = ++i;
                while(input[i] != '}' && input[i] != '\0'){
                    i++;
                }

                if(input[i] == '}'){
                    var_name = strndup(input + var_start, i - var_start);
                    i++;
                }
            } else {
                int var_start = i;
                while(isalnum(input[i]) || input[i] == '_'){
                    i++;
                }

                var_name = strndup(input + var_start, i - var_start);
            }

            /*while(isalnum(input[i]) || input[i] == '_'){
                i++;
            }

            char *var_name = strndup(input + start + 1, i - start - 1);*/

            if(var_name){
                char *var_value = getenv(var_name);
                if(var_value){
                    size_t value_len = strlen(var_value);
                    expanded_value = realloc(expanded_value, total_len + value_len + 1);
                    strcpy(expanded_value + total_len, var_value);
                    total_len += value_len;
                }

                free(var_name);
            }
        }

        if(expanded_value){
            (*tokens)[num_tokens].type = TOKEN_ARGUMENT;
            (*tokens)[num_tokens].value = expanded_value;
            num_tokens++;
            continue;
        }

        if(input[i] == '|'){
            (*tokens)[num_tokens].type = TOKEN_PIPE;
            (*tokens)[num_tokens].value = strndup(input + i, 1);
            num_tokens++;
            i++;
            expect_command = true;
            continue;
        } else {
            int start = i;
            while(input[i] != '\0' && !isspace(input[i]) && input[i] != '|'){
                if(input[i] == '\\' && (input[i + 1] == '\'' || input[i + 1] == '"')){
                    i += 2;
                    continue;
                }
                i++;
            }

            (*tokens)[num_tokens].type = expect_command ? TOKEN_COMMAND : TOKEN_ARGUMENT;
            (*tokens)[num_tokens].value = strndup(input + start, i - start);
            num_tokens++;
            expect_command = false;
        }

        i++;
    }

    return num_tokens;
}

void print_token(TokenType type){
    switch (type){
        case TOKEN_COMMAND: printf("COMMAND"); break;
        case TOKEN_ARGUMENT: printf("ARGUMENT"); break;
        case TOKEN_SINGLE_QUOTE_STRING: printf("SINGLE_QUOTE_STRING"); break;
        case TOKEN_DOUBLE_QUOTE_STRING: printf("DOUBLE_QUOTE_STRING"); break;
        case TOKEN_PIPE: printf("PIPE"); break;
        case TOKEN_VARIABLE: printf("VARIABLE"); break;
        default: printf("UNKNOWN"); break;
    }
}