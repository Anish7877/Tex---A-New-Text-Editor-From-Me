#include "terminal.h"
#include "tux.h"
#include <cstdio>

termios terminal::orig_termios{};
winsize terminal::win{};
void disable_Raw_Mode(){
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&terminal::orig_termios) == -1){
        terminal::die("tcsetattr");
    }
}
int terminal::get_cursor_position(int& rows, int& cols){
    std::vector<char> buffer(32);
    // this is the n escape sequence which is used to report the device status report
    // here we using code 6 which will give us cursor position report
    if(write(STDOUT_FILENO,"\x1b[6n",4) != 4) return -1;
    unsigned i{0};
    while(i < static_cast<unsigned>(buffer.size())-1){
        if(read(STDIN_FILENO,&buffer[i],1) != 1) break;
        if(buffer[i] == 'R') break;
        ++i;
    }
    buffer[i] = '\0';
    if(buffer[0] != '\x1b' || buffer[1] != '[') return -1;
    // here parsing the cursor report
    if(sscanf(&buffer[2],"%d;%d",&rows,&cols) != 2) return -1;
    editor::read_process();
    return -0;
}
int terminal::get_window_size(int& rows,int& cols){
    // here we used the C escape sequence which takes the cursor to rightmost
    // and we also used the B escape sequence with 999 code which takes the cursor to the down
    if(ioctl(STDOUT_FILENO,TIOCGWINSZ,&win) == -1 || win.ws_col == 0){
        if(write(STDOUT_FILENO,"\x1b[999C\x1b[999B",12) != 12) return -1;
        return get_cursor_position(rows,cols);
    }
    else{
        rows = win.ws_row;
        cols = win.ws_col;
    }
    return 0;
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
    write(STDOUT_FILENO,"\x1b[2J",4);
    write(STDOUT_FILENO,"\x1b[H",3);
    exit(1);
}
terminal::~terminal(){
}
