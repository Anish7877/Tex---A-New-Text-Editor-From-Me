#ifndef TUX_H
#define TUX_H
#define CTRL_Key(k) ((k)&0x1f)
#define TUX_VERSION "0.0.1"
#include "terminal.h"
#include <string_view>
#include <cerrno>
class append_buffer{
    public:
        append_buffer()=default;
        void append(std::string_view s);
        const char* get_buffer();
        int get_size();
        ~append_buffer();
    private:
        std::string buffer;
};
class editor{
    public:
        static bool running;
        editor()=default;
        static void init_editor();
        static void move_cursor(char& key);
        static void refresh_screen();
        static void draw_rows(append_buffer& buffer);
        static char read_process();
        static void process_input();
        ~editor();
    private:
        static int screen_rows;
        static int screen_cols;
        static int cursor_x;
        static int cursor_y;
};
#endif
