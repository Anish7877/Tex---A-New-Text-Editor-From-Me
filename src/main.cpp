#include "terminal.h"
#include "tux.h"

int main(int argc,char** argv){
    terminal term{};
    term.enable_Raw_Mode();
    editor::init_editor();
    while(1){
        editor::refresh_screen();
        editor::process_input();
    }
    return 0;
}
