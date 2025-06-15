#ifndef TERMINAL_H
#define TERMINAL_H
#include <string>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
void disable_Raw_Mode();
class terminal{
    public:
        static termios orig_termios;
        terminal() = default;
        static void enable_Raw_Mode();
        static void die(const std::string& s);
        ~terminal();
};
#endif
