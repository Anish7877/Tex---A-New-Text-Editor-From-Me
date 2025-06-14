#ifndef TEX_H
#define TEX_H
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
extern termios orig;
class piece_table{
};
void disable_Raw_Mode();
void enable_Raw_Mode();
#endif
