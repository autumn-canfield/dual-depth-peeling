#include"os.h"
#include"types.h"
#include<sys/stat.h>

   #define WIN32_LEAN_AND_MEAN
   #include<windows.h>
   #include<stdio.h>
   #include<stdarg.h>
typedef FILE* file_handle;

file_handle open_file(const char *file_name, const char *flags)
{
   return fopen(file_name, flags);
}

void close_file(file_handle f)
{
   fclose(f);
}

void file_write(file_handle f, u32 size, u32 count, void *data)
{
   fwrite(data, (size_t)size, (size_t)count, f);
}

void file_read(file_handle f, u32 size, u32 count, void *data)
{
   fread(data, (size_t)size, (size_t)count, f);
}

void file_printf(file_handle f, const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   vfprintf(f, fmt, args);
}

void file_scanf(file_handle f, const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   vfscanf(f, fmt, args);
}

void file_write_char(file_handle f, char c)
{
   fputc(c, f);
}

char file_read_char(file_handle f)
{
   return fgetc(f);
}

// Copies from file to data until a null character is
// reached. Does not copy the null character.
void file_read_string(file_handle f, u32 max_size, char *data)
{
   u32 i;
   for(i=0; i<max_size; ++i)
   {
      i32 c = fgetc(f);
      if((c==0)||(c==EOF))
         return;
      data[i]=(char)c;
   }
}

i32 file_size(file_handle f)
{
   fseek(f, 0, SEEK_END);
   i32 size = ftell(f);
   rewind(f);
   return size;
}

i32 file_get_pos(file_handle f)
{
   return ftell(f);
}

void file_set_pos(file_handle f, i32 position)
{
   fseek(f, position, SEEK_SET);
}

void file_inc_pos(file_handle f, i32 amount)
{
   fseek(f, amount, SEEK_CUR);
}

u64 file_write_time(const char *file_name)
{
   HANDLE file_h = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
         OPEN_EXISTING, 0, NULL);
   if(!file_h)
      return 0;
   FILETIME ftime_write;
   GetFileTime(file_h, NULL, NULL, &ftime_write);
   CloseHandle(file_h);
   u64 result = (((u64)ftime_write.dwHighDateTime) << 32 | ftime_write.dwLowDateTime);

   return result;
}

