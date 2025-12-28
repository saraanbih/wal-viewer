#include "ui.h"
#include "search.h"
#include <string.h>
#include <stdlib.h>

int ui_init(ui_state_t* state, const char* filename, wal_data_t* data)
{
   initscr();
   cbreak();
   noecho();
   keypad(stdscr, TRUE);
   curs_set(0);

   state->data = data;
   state->current_row = 0;
   state->scroll_offset = 0;
   state->mode = MODE_TEXT;
   state->filename = strdup(filename);

   int height, width;
   getmaxyx(stdscr, height, width);

   state->header_win = newwin(3, width, 0, 0);
   state->main_win = newwin(height - 5, width, 3, 0);
   state->footer_win = newwin(2, width, height - 2, 0);

   return 0;
}

static void draw_header(ui_state_t* state)
{
   werase(state->header_win);
   box(state->header_win, 0, 0);

   wattron(state->header_win, A_BOLD);
   mvwprintw(state->header_win, 1, 2, "WAL Viewer: %s", state->filename);
   
   int width = getmaxx(state->header_win);
   const char* mode_str = (state->mode == MODE_TEXT) ? "[TEXT]" : "[BINARY]";
   mvwprintw(state->header_win, 1, width - 12, "%s", mode_str);
   wattroff(state->header_win, A_BOLD);

   wrefresh(state->header_win);
}

static void draw_records(ui_state_t* state)
{
   werase(state->main_win);
   box(state->main_win, 0, 0);

   int height = getmaxy(state->main_win);
   //int width = getmaxx(state->main_win);

   // Header row
   wattron(state->main_win, A_BOLD | A_UNDERLINE);
   mvwprintw(state->main_win, 1, 2, "%-10s %-15s %-8s %s", 
             "Type", "LSN", "Length", "Description");
   wattroff(state->main_win, A_BOLD | A_UNDERLINE);

   // Records
   for (size_t i = 0; i < (size_t)(height - 3) && (i + state->scroll_offset) < state->data->count; i++)
   {
      int idx = i + state->scroll_offset;
      wal_record_t* rec = &state->data->records[idx];

      if (idx == state->current_row)
         wattron(state->main_win, A_REVERSE);

      if (state->mode == MODE_TEXT) {
         mvwprintw(state->main_win, i + 2, 2, "%-10s %-15s %-8d %.40s",
                   rec->type, rec->lsn, rec->length, rec->description);
      } else {
         mvwprintw(state->main_win, i + 2, 2, "%-10s %-15s %-8d %s",
                   rec->type, rec->lsn, rec->length, rec->binary_hex);
      }

      if (idx == state->current_row)
         wattroff(state->main_win, A_REVERSE);
   }

   wrefresh(state->main_win);
}

static void draw_footer(ui_state_t* state)
{
   werase(state->footer_win);
   box(state->footer_win, 0, 0);

   mvwprintw(state->footer_win, 0, 2, 
             "[↑↓] Navigate | [T]ext/[B]inary | [Enter] Details | [S]earch | [Q]uit");

   wrefresh(state->footer_win);
}

static void show_details(ui_state_t* state)
{
   wal_record_t* rec = &state->data->records[state->current_row];

   int height = 15, width = 70;
   int starty = (LINES - height) / 2;
   int startx = (COLS - width) / 2;

   WINDOW* detail_win = newwin(height, width, starty, startx);
   box(detail_win, 0, 0);

   wattron(detail_win, A_BOLD);
   mvwprintw(detail_win, 1, 2, "Record Details");
   wattroff(detail_win, A_BOLD);

   mvwprintw(detail_win, 3, 2, "Type:        %s", rec->type);
   mvwprintw(detail_win, 4, 2, "LSN:         %s", rec->lsn);
   mvwprintw(detail_win, 5, 2, "Length:      %d bytes", rec->length);
   mvwprintw(detail_win, 7, 2, "Description:");
   mvwprintw(detail_win, 8, 4, "%s", rec->description);
   mvwprintw(detail_win, 10, 2, "Binary (hex):");
   mvwprintw(detail_win, 11, 4, "%s", rec->binary_hex);

   mvwprintw(detail_win, height - 2, 2, "Press any key to close...");
   wrefresh(detail_win);
   getch();
   delwin(detail_win);
}

void ui_run(ui_state_t* state)
{
   int ch;
   int height = getmaxy(state->main_win);

   while (1)
   {
      draw_header(state);
      draw_records(state);
      draw_footer(state);

      ch = getch();

      switch (ch)
      {
         case KEY_UP:
            if (state->current_row > 0) {
               state->current_row--;
               if (state->current_row < state->scroll_offset)
                  state->scroll_offset--;
            }
            break;

         case KEY_DOWN:
            if ((size_t)state->current_row < state->data->count - 1) {
               state->current_row++;
               if (state->current_row >= state->scroll_offset + height - 3)
                  state->scroll_offset++;
            }
            break;

         case 't':
         case 'T':
            state->mode = MODE_TEXT;
            break;

         case 'b':
         case 'B':
            state->mode = MODE_BINARY;
            break;

         case 10:  // Enter
            show_details(state);
            break;

         case 's':
         case 'S':
            search_interactive(state);
            break;

         case 'q':
         case 'Q':
            return;
      }
   }
}

void ui_cleanup(ui_state_t* state)
{
   delwin(state->header_win);
   delwin(state->main_win);
   delwin(state->footer_win);
   endwin();
   free(state->filename);
}