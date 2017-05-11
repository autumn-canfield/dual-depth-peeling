#ifndef _FILE_IO_H
#define _FILE_IO_H

#include"types.h"
#include<stdio.h>

typedef FILE* file_handle;

file_handle open_file(const char *file_name, const char *flags );
void close_file(file_handle f);
void file_write(file_handle f, u32 size, u32 count, void *data);
void file_read(file_handle f, u32 size, u32 count, void *data);
void file_printf(file_handle f, const char *fmt, ...);
void file_scanf(file_handle f, const char *fmt, ...);
void file_write_char(file_handle f, char c);
char file_read_char(file_handle f);
void file_read_string(file_handle f, u32 max_size, char *data);
i32 file_size(file_handle f);
i32 file_get_pos(file_handle f);
void file_set_pos(file_handle f, i32 position);
void file_inc_pos(file_handle f, i32 amount);
u64 file_write_time(const char *file_name);

#endif
