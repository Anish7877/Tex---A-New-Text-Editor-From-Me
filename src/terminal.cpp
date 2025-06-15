#include "terminal.h"
#include <iostream>

termios terminal::orig_termios{};
void disable_Raw_Mode(){
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&terminal::orig_termios) == -1){
        terminal::die("tcsetattr");
    }
}
void terminal::enable_Raw_Mode(){
    // this is like turning off the echoing of the terminal when we press a key
    // then it shows us on the screen if we turn off the echo and we will also
    // turn off canonical mode of terminal (ICANON)
    // canonical mode -> The data is available to the program after ctrl-d is pressed
    // until ctrl-d is not pressed data is not present in the program.
    // non-canonical mode -> The data is available as entered in the program like
    // if a is pressed a is present to the a for the program to process.
    // for ctrl-c and ctrl-z signals we should also stop signals (ISIG)
    // also disable ctrl-s and ctrl-q to stop software control flow (IXON)
    // disable ctrl-v also on some system terminal waits for a char input (IEXTEN)
    // also disable ctrl-m because both enter and ctrl-m read as 13 (ICRNL)
    // turn off output processing also (OPOST)
    // some miscellaneous flags
    // 1. BRKINT -> When BRKINT is turned on, a break condition will cause a SIGINT
    // signal to be sent to the program, like pressing Ctrl-C.
    // 2. INPCK -> it enables parity checking
    // 3. ISTRIP -> causes 8th bit of character to be stripped i.e. set to 0
    // 4. CS8 -> it is a bitmask which sets all character to be 8 bits
    // A time out for read -> now read() reads indefinitely from the keyboard but we can
    // time the read() to return if no character are being input from keyboard
    if(tcgetattr(STDIN_FILENO,&orig_termios) == -1) die("tcgetattr");
    atexit(disable_Raw_Mode);
    termios raw{orig_termios};
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw) == -1) die("tcsetattr");
}
void terminal::die(const std::string& s){
    std::cerr << s << '\n';
    exit(1);
}
terminal::~terminal(){
}
