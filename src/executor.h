#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "tokenizer.h"

typedef struct {
    Token *tokens;
    int num_tokens;
} Command;

int execute(Token *tokens, int num_tokens);

#endif