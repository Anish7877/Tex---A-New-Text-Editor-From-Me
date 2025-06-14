#include <iostream>
#include "tex.h"

int main(int argc,char** argv){
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    enable_Raw_Mode();
    char c{};
    while(read(STDIN_FILENO,&c,1) == 1 && c != 'q'){
    }
    return 0;
}
