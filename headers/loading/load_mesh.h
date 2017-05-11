#ifndef _LOAD_MESH_H
#define _LOAD_MESH_H

typedef struct{
   u32 num_vertices;
   u32 num_indices;
   f32 *vertices;
   u32 *indices;
}loaded_mesh;

loaded_mesh load_mesh(const char *file_name);

#endif
