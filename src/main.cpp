#include "terminal.h"
#include "tux.h"

int main(int argc,char** argv){
    terminal::enable_Raw_Mode();
    editor::init_editor();
    while(editor::running){
        editor::refresh_screen();
        editor::process_input();
    }
    return 0;
}
