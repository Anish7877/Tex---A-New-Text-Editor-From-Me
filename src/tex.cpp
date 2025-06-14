#include "tex.h"

void disable_Raw_Mode(){
    // after editor is closed terminal needs to be reset
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&orig);
}
void enable_Raw_Mode(){
    // this is like turning off the echoing of the terminal when we press a key
    // then it shows us on the screen if we turn off the echo and we will also
    // turn off canonical mode of terminal
    // canonical mode -> The data is available to the program after ctrl-d is pressed
    // until ctrl-d is not pressed data is not present in the program.
    // non-canonical mode -> The data is available as entered in the program like
    // if a is pressed a is present to the a for the program to process.
    tcgetattr(STDIN_FILENO,&orig);
    atexit(disable_Raw_Mode);
    termios raw{orig};
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
}
