//CS431: Persistence Assignment - Anna Vadella
#include <stdint.h>

#ifndef ZIPPER_H
#define ZIPPER_H

// To force compiler to use 1 byte packaging
#pragma pack(1)

typedef struct eocd 
{
    int signature;                                              //0x06054b50
    uint16_t disk_number;
    uint16_t start_disk_number;
    uint16_t disk_cd_records;
    uint16_t total_cd_records;
    int cd_size;
    int cd_offset;
    uint16_t comment_length;
    //char *comment;
} EOCD;

typedef struct central_dir_file_header 
{
    int signature;                                              // 0x02014b50
    uint16_t made_version;                                      // 0x031e
    uint16_t min_version;                                       // 0x000a
    uint16_t bit_flag;                                          //0x0000
    uint16_t compression;                                       // none = 0x0000, deflate = 0x0800
    uint16_t mod_time;
    uint16_t mod_date;
    int crc;
    int compressed_size;
    int uncompressed_size;
    uint16_t filename_length;
    uint16_t extra_field_length;
    uint16_t comment_length;
    uint16_t start_disk;
    uint16_t internal_file_attrs;
    int external_file_attrs;
    int local_header_offset;
    //char *filename;
    //char *extra_field;
    //char *comment;
} CDHEADER;

typedef struct local_file_header 
{
    int signature;                                              // 0x04034b50
    uint16_t min_version;                                       //0x000a
    uint16_t bit_flag;
    uint16_t compression;                                       // none = 0x0000, deflate = 0x0800
    uint16_t mod_time;
    uint16_t mod_date;
    int crc;
    int compressed_size;
    int uncompressed_size;
    uint16_t filename_length;
    uint16_t extra_field_length;
    //char *filename;
    //char *extra_field;
} HEADER;

void create_file(char* filename, char** files, int numfiles);
int add_file(FILE* zf, char* filename, HEADER* lh, int offset);

#endif