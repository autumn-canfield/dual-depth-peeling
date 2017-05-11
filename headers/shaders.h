#ifndef _SHADERS_H
#define _SHADERS_H

#include"types.h"

u32 link_program (u32 vs, u32 fs);
u32 load_fragment_shader(const char *file_name);
u32 load_vertex_shader(const char *file_name);
u32 reload_fragment_shader(const char *file_name, u32 shader_id);
u32 reload_vertex_shader(const char *file_name, u32 shader_id);

#endif // _SHADERS_H

