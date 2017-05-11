#include<stdio.h>
#include<string.h>
#include<GL/glew.h>
#include"util/memory/alloc.h"
#include"util/file_io.h"
#include"types.h"

static const char *_default_fs =
"#version 330 core\n"
"out vec3 color;\n"
"void main(){\n"
"   color = vec3(0.7,0.1,0.9);\n"
"}";

static const char *_default_vs =
"#version 330 core\n"
"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
"void main(){\n"
"   vec3 pos = vertexPosition_modelspace;\n"
"   gl_Position = vec4(pos, 1.0);\n"
"}";

static u8 compile_shader(const char *shader_source, i32 size, u32 shader_id)
{
   glShaderSource(shader_id, 1, (const char**)&shader_source, &size);
   glCompileShader(shader_id);

   i32 compile_status;
   glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

   return compile_status;
}

static void print_shader_compilation_log(u32 shader_id)
{
   i32 info_log_length;
   glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
   char *info_log = alloc(info_log_length+1);
   info_log[info_log_length] = 0;
   glGetShaderInfoLog(shader_id, info_log_length, NULL, info_log);
   printf("%s", info_log);
   dealloc(info_log);
}

static void print_program_compilation_log(u32 program_id)
{
   i32 info_log_length;
   glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
   char *info_log = alloc(info_log_length+1);
   info_log[info_log_length] = 0;
   glGetProgramInfoLog(program_id, info_log_length, NULL, info_log);
   printf("%s\n", info_log);
   dealloc(info_log);
}

static void load_shader(const char *file_name, u32 shader_id, const char *fallback)
{
   i32 size;
   file_handle shader_file = open_file(file_name, "rb");
   if(!shader_file)
   {
      printf("Couldn't open %s!\n", file_name);
      print_shader_compilation_log(shader_id);
      compile_shader(fallback, 0, shader_id);
   }

   size = file_size(shader_file);
   char *shader_source = alloc(size);
   file_read(shader_file, sizeof(char), size, shader_source);
   close_file(shader_file);

   u8 compile_result = compile_shader(shader_source, size, shader_id);

   if(compile_result==GL_FALSE)
   {
      printf("Shader %s failed to compile!\n", file_name);
      compile_shader(fallback, 0, shader_id);
   }
      print_shader_compilation_log(shader_id);
}

u32 link_program (u32 vs, u32 fs)
{
   u32 program_id = glCreateProgram();
   glAttachShader(program_id, vs);
   glAttachShader(program_id, fs);
   glLinkProgram(program_id);

   i32 compile_status;
   glGetProgramiv(program_id, GL_LINK_STATUS, &compile_status);
   if(compile_status==GL_FALSE)
   {
      printf("Shader program could not link!\n");
   }
      print_program_compilation_log(program_id);
   return program_id;
}

void reload_fragment_shader(const char *file_name, u32 shader_id)
{
   load_shader(file_name, shader_id, _default_fs);
}

void reload_vertex_shader(const char *file_name, u32 shader_id)
{
   load_shader(file_name, shader_id, _default_vs);
}

u32 load_fragment_shader(const char *file_name)
{
   u32 shader_id = glCreateShader(GL_FRAGMENT_SHADER);
   load_shader(file_name, shader_id, _default_fs);
   return shader_id;
}

u32 load_vertex_shader(const char *file_name)
{
   u32 shader_id = glCreateShader(GL_VERTEX_SHADER);
   load_shader(file_name, shader_id, _default_vs);
   return shader_id;
}

