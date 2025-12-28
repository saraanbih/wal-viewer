#include "search.h"
#include "ui.h"
#include <string.h>
#include <ncurses.h>

void search_interactive(ui_state_t* state)
{
   char search_term[100] = {0};

   // Create search window
   WINDOW* search_win = newwin(5, 60, LINES/2 - 2, COLS/2 - 30);
   box(search_win, 0, 0);
   mvwprintw(search_win, 1, 2, "Search: ");
   wrefresh(search_win);

   echo();
   curs_set(1);
   mvwgetnstr(search_win, 1, 10, search_term, 99);
   noecho();
   curs_set(0);

   if (strlen(search_term) == 0) {
      delwin(search_win);
      return;
   }

   // Search from current position
   int start = state->current_row + 1;
   for (size_t i = 0; i < state->data->count; i++)
   {
      int idx = (start + i) % state->data->count;
      wal_record_t* rec = &state->data->records[idx];

      if (strstr(rec->type, search_term) ||
          strstr(rec->description, search_term))
      {
         state->current_row = idx;
         // Adjust scroll
         int height = getmaxy(state->main_win) - 3;
         if (idx < state->scroll_offset)
            state->scroll_offset = idx;
         else if (idx >= state->scroll_offset + height)
            state->scroll_offset = idx - height + 1;
         break;
      }
   }

   delwin(search_win);
}