#include "tux.h"

/** append_buffer **/
void append_buffer::append(std::string_view s){
    buffer.append(s);
}
const char* append_buffer::get_buffer(){
    return buffer.c_str();
}
int append_buffer::get_size(){
    return buffer.size();
}
append_buffer::~append_buffer(){
}

/** editor **/
int editor::screen_rows{};
int editor::screen_cols{};
void editor::init_editor(){
    if(terminal::get_window_size(screen_rows,screen_cols) == -1) terminal::die("Window Size");
}
void editor::draw_rows(append_buffer& buffer){
    for(int i{0};i<screen_rows;++i){
        if(i == screen_rows/3){
            std::string welcome_message{"Tux - Text Editor version"};
        }
        else{
            buffer.append("~");
        }
        // K escape sequence is used which clear each line one at a time
        buffer.append("\x1b[K");
        if(i < screen_rows-1){
            buffer.append("\r\n");
        }
    }
}
void editor::refresh_screen(){
    // here we are writing 4 bytes. This is a escape sequence
    // always starts with escape character
    // 1. \x1b which is 27 in decimal and escape character
    // 2. [ this is escape sequence instruct
    //      this can be used to do many things in the terminal
    //      like moving cursor, coloring text etc.
    // 3. J command i.e. erase in display command which is used with
    //    particular code i.e. 0,1,2 here we are using 2 to clear
    //    all display
    append_buffer buffer{};
    buffer.append("\x1b[?25l");
    // buffer.append("\x1b[2J"); this is not feasible to repaint the screen again and again
    // do we should only clear one a time
    // repositioning the cursor
    // here we will write 3 bytes using the escape sequence
    // 1. \x1b -> escape character
    // 2. [ -> escape sequence instruct
    // 3. H -> this repositions the cursor
    buffer.append("\x1b[H");
    draw_rows(buffer);
    buffer.append("\x1b[H");
    buffer.append("\x1b[?25l");
    write(STDOUT_FILENO,buffer.get_buffer(),buffer.get_size());
}
char editor::read_process(){
    int nread{};
    char c{};
    while((nread = read(STDIN_FILENO,&c,1)) != -1){
        if(nread == -1 && errno != EAGAIN) terminal::die("read");
    }
    return c;
}
void editor::process_input(){
    char c{read_process()};
    switch(c){
        case CTRL_Key('q'):
            write(STDOUT_FILENO,"\x1b[2J",4);
            write(STDOUT_FILENO,"\x1b[H",3);
            exit(0);
            break;
    }
}
editor::~editor(){
}
