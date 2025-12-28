#ifndef WAL_PARSER_H
#define WAL_PARSER_H

#include <stddef.h>

typedef struct {
    char type[16]; //XLOG, HEAP, BTREE, etc.
    char lsn[32]; //log sequence number
    int length; //length of the WAL record
    char description[256]; //description of the WAL record
    char binary_hex[1024]; //hex representation of the binary data
}wal_record_t;

typedef struct {
    wal_record_t* records; //dynamic array of WAL records
    size_t count; //number of WAL records stored
    size_t capacity; //capacity of the dynamic array
}wal_data_t;

int wal_parse_file(const char* filepath, wal_data_t* wal_data); // Parses the WAL file and fills the wal_data structure
void wal_data_free(wal_data_t* wal_data); // frees the allocated memory for wal_data
void wal_record_to_binary(wal_record_t *record); // converts the hex representation back to binary data

#endif //WAL_PARSER_H