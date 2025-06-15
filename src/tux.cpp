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
int editor::cursor_x{};
int editor::cursor_y{};
bool editor::running{true};
void editor::init_editor(){
    cursor_x = 0;
    cursor_y = 0;
    if(terminal::get_window_size(screen_rows,screen_cols) == -1) terminal::die("Window Size");
}
void editor::move_cursor(char& key){
    switch(key){
        case Up:
            if(cursor_y < screen_rows-1){
                cursor_y++;
            }
            break;
        case Down:
            if(cursor_y > 0){
                cursor_y--;
            }
            break;
        case Left:
            if(cursor_x > 0){
                cursor_x--;
            }
            break;
        case Right:
            if(cursor_x < screen_cols-1){
                cursor_x++;
            }
            break;
        default:
            break;
    }
}
void editor::draw_rows(append_buffer& buffer){
    for(int i{0};i<screen_rows;++i){
        if(i == screen_rows/3){
            // welcome message will be printed when turned on
            std::string welcome_message{"Tux - Text Editor Version "};
            welcome_message += TUX_VERSION;
            size_t welcome_len = welcome_message.size();
            if(welcome_len > (size_t)screen_cols) welcome_len = screen_cols;
            int padding{static_cast<int>((screen_cols-welcome_len))/2};
            if(padding){
                buffer.append("~");
                --padding;
            }
            while(padding--) buffer.append(" ");
            buffer.append(welcome_message);
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
    //buffer.append("\x1b[?25l");
    // buffer.append("\x1b[2J"); this is not feasible to repaint the screen again and again
    // do we should only clear one a time
    // repositioning the cursor
    // here we will write 3 bytes using the escape sequence
    // 1. \x1b -> escape character
    // 2. [ -> escape sequence instruct
    // 3. H -> this repositions the cursor
    append_buffer buffer{};
    buffer.append("\x1b[H");
    draw_rows(buffer);
    std::string move_cursor{"\x1b["};
    move_cursor += std::to_string(cursor_y+1);
    move_cursor += ';';
    move_cursor += std::to_string(cursor_x+1);
    move_cursor += 'H';
    buffer.append(move_cursor);
    //buffer.append("\x1b[?25l");
    write(STDOUT_FILENO,buffer.get_buffer(),buffer.get_size());
}
char editor::read_process(){
    int nread{};
    char c{};
    nread = read(STDIN_FILENO,&c,1);
    if(nread == -1 && errno != EAGAIN){
        terminal::die("read");
    }
    if(nread == 0){
        return '\0';
    }
    if(c == '\x1b'){
        std::vector<char> seq(3);
        if(read(STDIN_FILENO,&seq[0],1) != 1) return '\x1b';
        if(read(STDIN_FILENO,&seq[1],1) != 1) return '\x1b';

        if(seq[0] == '['){
            switch(seq[1]){
                case 'A' : return Down;
                case 'B' : return Up;
                case 'C' : return Right;
                case 'D' : return Left;
                default: break;
            }
        }
        return '\x1b';
    }
    return c;
}
void editor::process_input(){
    char c{read_process()};
    if(c == '\0') return;
    switch(c){
        case CTRL_Key('q'):
            write(STDOUT_FILENO,"\x1b[2J",4);
            write(STDOUT_FILENO,"\x1b[H",3);
            running = false;
            break;
        case Up:
        case Down:
        case Left:
        case Right:
            move_cursor(c);
            break;
        default:
            break;
    }
}
editor::~editor(){
}
