#ifndef TOKENIZER_H
#define TOKENIZER_H

typedef enum {
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_VARIABLE,
    TOKEN_SINGLE_QUOTE_STRING,
    TOKEN_DOUBLE_QUOTE_STRING,
    TOKEN_PIPE
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} Token;

int tokenize(char *input, Token **tokens);
void print_token(TokenType type);

#endif