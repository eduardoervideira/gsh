#include <termios.h>
#include <stdio.h>
#include <unistd.h>

void set_raw_mode(struct termios *old_termios){
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, old_termios);
    new_termios = *old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void reset_terminal(struct termios *old_termios){
    printf("\033[0 q");
    tcsetattr(STDIN_FILENO, TCSANOW, old_termios);
}