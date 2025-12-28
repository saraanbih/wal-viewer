#ifndef UI_H
#define UI_H

#include "wal_parser.h"
#include <ncurses.h>

typedef enum {
    MODE_TEXT,
    MODE_BINARY
} display_mode_t;

typedef struct {
    wal_data_t* data;
    int current_row;
    int scroll_offset;
    display_mode_t mode;
    WINDOW* main_win;
    WINDOW* header_win;
    WINDOW* footer_win;
    char* filename;
} ui_state_t;

int ui_init(ui_state_t* state, const char* filename, wal_data_t* data);
void ui_run(ui_state_t* state);
void ui_cleanup(ui_state_t* state);

#endif