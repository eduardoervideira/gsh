#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stddef.h>

#define MAX_INPUT_LENGTH 1024

int get_input(char *input, size_t *cursor_pos, size_t *input_length);

#endif