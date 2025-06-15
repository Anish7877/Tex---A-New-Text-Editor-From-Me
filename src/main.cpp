#include <cerrno>
#include <iostream>
#include "terminal.h"

int main(int argc,char** argv){
    terminal term{};
    term.enable_Raw_Mode();
    while(1){
        char c{'\0'};
        if(read(STDIN_FILENO,&c,1) == -1 && errno == EAGAIN) terminal::die("read");
        if(iscntrl(c)){
            std::cout << c << char(13) << '\n';
        }
        else{
            std::cout << (int)c << '(' << c << ')' << char(13) << '\n';
        }
        if(c == 'q') break;
    }
    return 0;
}
