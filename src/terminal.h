#ifndef TERMINAL_H
#define TERMINAL_H
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <vector>
void disable_Raw_Mode();
class terminal{
    public:
        static termios orig_termios;
        terminal() = default;
        static void enable_Raw_Mode();
        static void die(const std::string& s);
        static int get_cursor_position(int& rows,int& cols);
        static int get_window_size(int& rows,int& cols);
        ~terminal();
    private:
        static winsize win;
};
#endif
