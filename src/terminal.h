#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

void set_raw_mode(struct termios *old_termios);
void reset_terminal(struct termios *old_termios);

#endif