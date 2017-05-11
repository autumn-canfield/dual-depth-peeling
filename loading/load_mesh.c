#include"util/memory/alloc.h"
#include"util/file_io.h"
#include"types.h"

typedef struct{
   u32 num_vertices;
   u32 num_indices;
   f32 *vertices;
   u32 *indices;
}loaded_mesh;

loaded_mesh load_mesh(const char *file_name)
{
   file_handle mesh_file = open_file(file_name, "rb");
   loaded_mesh mesh;

   file_read(mesh_file, sizeof(u32), 1, &mesh.num_vertices);
   mesh.vertices = alloc(mesh.num_vertices*3*sizeof(f32));
   file_read(mesh_file, sizeof(f32), mesh.num_vertices*3, mesh.vertices);

   file_read(mesh_file, sizeof(u32), 1, &mesh.num_indices);
   mesh.indices = alloc(mesh.num_indices*sizeof(u32));
   file_read(mesh_file, sizeof(u32), mesh.num_indices, mesh.indices);

   close_file(mesh_file);
   return mesh;
}

