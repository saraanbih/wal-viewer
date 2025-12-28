#include <stdio.h>
#include "wal_parser.h"
#include "ui.h"  
#include "search.h"
#include "utils.h"

int main(int argc, char** argv)
{
   if (argc < 2) {
      fprintf(stderr, "Usage: %s <wal_file>\n", argv[0]);
      return 1;
   }

   // Parse WAL data
   wal_data_t data = {0};
   if (wal_parse_file(argv[1], &data) != 0) {
      fprintf(stderr, "Failed to parse WAL file: %s\n", argv[1]);
      return 1;
   }

   printf("Loaded %zu records\n", data.count);

   // Initialize UI
   ui_state_t state = {0};
   if (ui_init(&state, argv[1], &data) != 0) {
      fprintf(stderr, "Failed to initialize UI\n");
      wal_data_free(&data);
      return 1;
   }

   // Run interactive mode
   ui_run(&state);

   // Cleanup
   ui_cleanup(&state);
   wal_data_free(&data);

   return 0;
}