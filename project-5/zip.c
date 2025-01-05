//CS431: Persistence Assignment - Anna Vadella
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <zlib.h>

#include "zip.h"

void create_file(char* filename, char** files, int numfiles) 
{
    FILE* f = fopen(filename, "wb");
    if(!f) 
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    int offset = 0;                                                     // to track position as the file is written
    int len_names = 0;                                                  // sum of the size of filenames for CD size
    HEADER headers[numfiles];                                           // local headers
    int offsets[numfiles];                                              // offsets to each file

    for(int i = 0; i < numfiles; i++) 
    {
        printf("zipping %s\n", files[i]);
        // store the current offset, add to len_names, add_file()
        offsets[i] = offset;
        offset += add_file(f, files[i], &headers[i], offset);
        len_names += strlen(files[i]);
    }

    for(int i = 0; i < numfiles; i++) 
    {
        // create and write the central directory entries
        CDHEADER cdh;
        cdh.signature = 0x02014b50;
        cdh.made_version = 0x031e;
        cdh.min_version = 0x000a;
        cdh.bit_flag = 0;                                               // 0x0000
        cdh.compression = 0;                                            // none = 0x0000, deflate = 0x0800
        cdh.mod_time = headers[i].mod_time;
        cdh.mod_date = headers[i].mod_date;
        cdh.crc = headers[i].crc;
        cdh.compressed_size = headers[i].compressed_size;
        cdh.uncompressed_size = headers[i].uncompressed_size;
        cdh.filename_length = headers[i].filename_length;
        cdh.extra_field_length = 0;
        cdh.comment_length = 0;
        cdh.start_disk = 0;
        cdh.internal_file_attrs = 0;
        cdh.external_file_attrs = 0;
        cdh.local_header_offset = offsets[i];

        fwrite(&cdh, sizeof(CDHEADER), 1, f);
        fprintf(f, "%s", files[i]);
    }

    // create and write the end-of-central-directory
    EOCD cdir;
    cdir.signature = 0x06054b50;
    cdir.disk_number = 0;
    cdir.start_disk_number = 0;
    cdir.disk_cd_records = numfiles;
    cdir.total_cd_records = numfiles;
    cdir.cd_size = (sizeof(CDHEADER) * numfiles) + len_names;
    cdir.cd_offset = offset;
    cdir.comment_length = 0;

    fwrite(&cdir, sizeof(EOCD), 1, f);
    fclose(f);
}


int add_file(FILE* zf, char* filename, HEADER* lh, int offset) 
{
    // open the file to add to the zip
    FILE* f = fopen(filename, "rb");
    if(!f) 
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // get the stat() info
    struct stat sb;
    // stat() here ...
    stat(filename, &sb);                                                //stat() is a system call!

    // get the contents
    char* file_contents = malloc(sb.st_size);
    fread(file_contents, sb.st_size, 1, f);

    fclose(f);
    
    // dos date
    // day:5b, month:4b, year:7b
    uint16_t dosdate = (localtime(&(sb.st_mtime))->tm_year - 80);
    dosdate = dosdate << 4;
    dosdate += (localtime(&(sb.st_mtime))->tm_mon + 1);
    dosdate = dosdate << 5;
    dosdate += localtime(&(sb.st_mtime))->tm_mday;
    // printf("dosdate: %x\n", dosdate);

    // dos time
    // hour:5b min:6b sec:5b
    uint16_t dostime = (localtime(&(sb.st_mtime))->tm_hour);
    dostime = dostime << 6;
    dostime += (localtime(&(sb.st_mtime))->tm_min);
    dostime = dostime << 5;
    dostime += localtime(&(sb.st_mtime))->tm_sec /2;
    // printf("dostime: %x\n", dostime);


    uint32_t crc = crc32(0, (const Bytef*)file_contents, strlen(file_contents));
    // printf("crc: %x\n", crc);

    // create the local header
    lh->signature = 0x04034b50;
    lh->min_version = 0x000a;                                           //0x000a
    lh->bit_flag = 0;
    lh->compression = 0;                                                // none = 0x0000, deflate = 0x0800
    lh->mod_time = dostime;
    lh->mod_date = dosdate;
    lh->crc = crc;
    lh->compressed_size = sb.st_size;
    lh->uncompressed_size = sb.st_size;
    lh->filename_length = strlen(filename);
    lh->extra_field_length = 0;
    
    fwrite(lh, sizeof(HEADER), 1, zf);                                  // write the local header
    fprintf(zf, "%s", filename);                                        // write the file name
    fwrite(file_contents, sb.st_size, 1, zf);                           // write the file contents

    free(file_contents);
    return sb.st_size + sizeof(HEADER) + strlen(filename);
}