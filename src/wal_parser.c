#include "wal_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wal_parse_file(const char* filename, wal_data_t* data)
{
   FILE* fp = fopen(filename, "r");
   if (!fp) {
      return -1;
   }

   // Initialize
   data->capacity = 100;
   data->count = 0;
   data->records = malloc(sizeof(wal_record_t) * data->capacity);

   char line[512];
   while (fgets(line, sizeof(line), fp))
   {
      // Expand if needed
      if (data->count >= data->capacity) {
         data->capacity *= 2;
         data->records = realloc(data->records, 
                                sizeof(wal_record_t) * data->capacity);
      }

      wal_record_t* rec = &data->records[data->count];

      // Parse: XLOG|0/1000028|114|CHECKPOINT_SHUTDOWN...
      char* token = strtok(line, "|");
      if (token) strncpy(rec->type, token, sizeof(rec->type) - 1);

      token = strtok(NULL, "|");
      if (token) strncpy(rec->lsn, token, sizeof(rec->lsn) - 1);

      token = strtok(NULL, "|");
      if (token) rec->length = atoi(token);

      token = strtok(NULL, "\n");
      if (token) strncpy(rec->description, token, sizeof(rec->description) - 1);

      // Generate fake binary for now
      wal_record_to_binary(rec);

      data->count++;
   }

   fclose(fp);
   return 0;
}

void wal_data_free(wal_data_t* data)
{
   free(data->records);
   data->records = NULL;
   data->count = 0;
}

void wal_record_to_binary(wal_record_t* rec)
{
   // Generate fake hex for demonstration
   snprintf(rec->binary_hex, sizeof(rec->binary_hex),
            "4A 7F 8B 3C FF A1 23 45 67 89 AB CD EF 01 23 45");
}