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

        size_t start = 1;
        size_t end = len;
        if(token[1] == '{'){
            start++;
            end--;
        }

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
            i++;
            size_t start = i;

            while(input[i] != '\0' && input[i] != quote){
                i++;
            }

            if(input[i] == '\0'){
                fprintf(stderr, "unterminated quote\n");
                return -1;
            }

            if(quote == '"'){
                // TODO: expand variables inside double quotes

            }

            size_t end = i;
            char *value = strndup(input + start, end - start);
            (*tokens)[num_tokens].value = value;
            (*tokens)[num_tokens].type = TOKEN_ARGUMENT;
            num_tokens++;
            i++;
            continue;
        } else if(input[i] == '|'){
            if(expect_command){
                fprintf(stderr, "unexpected pipe\n");
                return -1;
            }

            (*tokens)[num_tokens].value = strdup("|");
            (*tokens)[num_tokens].type = TOKEN_PIPE;
            num_tokens++;
            i++;
            expect_command = true;
            continue;
        } else if(input[i] == '$'){
            bool expect_braces = false;
            size_t start = i;
            i++;

            if(input[i] == '{'){
                i++; // skip '{'
                start++;
                expect_braces = true;
            }

            while(isalnum(input[i]) || input[i] == '_'){
                i++;
            }

            if(input[i] == '}'){
                if(!expect_braces){
                    fprintf(stderr, "unexpected '}'\n");
                    return -1;
                }

                i++;
                expect_braces = false;
            }

            if(expect_braces){
                fprintf(stderr, "unterminated variable\n");
                return -1;
            }

            char *var = strndup(input + start, i - start);
            if(strlen(var) == 0)
                return -1;

            char *expanded = expand_variable(var);
            free(var);

            if(strlen(expanded) == 0)
                continue;

            (*tokens)[num_tokens].value = expanded;
            (*tokens)[num_tokens].type = TOKEN_ARGUMENT;
            num_tokens++;
            continue;
        } else {
            size_t start = i;
            while(input[i] != '\0' && !isspace(input[i]) && input[i] != '|'){
                i++;
            }
            size_t end = i;

            if(expect_command){
                (*tokens)[num_tokens].value = strndup(input + start, end - start);
                (*tokens)[num_tokens].type = TOKEN_COMMAND;
                num_tokens++;
                expect_command = false;
            } else {
                (*tokens)[num_tokens].value = strndup(input + start, end - start);
                (*tokens)[num_tokens].type = TOKEN_ARGUMENT;
                num_tokens++;
            }
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
        default: printf("UNKNOWN"); break;
    }
}