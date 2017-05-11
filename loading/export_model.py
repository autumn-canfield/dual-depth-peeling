import bpy
import time

t0 = time.clock()

def trianglulate_mesh(m):
   import bmesh
   bm = bmesh.new()
   bm.from_mesh(m)
   bmesh.ops.triangulate(bm, faces=bm.faces)
   bm.to_mesh(m)
   bm.free()

def binary_float(f):
   import struct
   # For big-endian use '>f' instead.
   return struct.pack('<f', f)

def binary_u32(i):
   import struct
   return struct.pack('<I', i)

file = open('../../resources/mesh', 'wb')

for obj in bpy.context.scene.objects:
   if obj.type != 'MESH':
      continue
   
   mesh = obj.to_mesh(bpy.context.scene, True, 'PREVIEW',
         calc_tessface=False)
   trianglulate_mesh(mesh)


   #from array import array
   #vertices_array = array('f', mesh.vertices.values())
   #vertices_array.tofile(file)

   t1 = time.clock()
   file.write(binary_u32(len(mesh.vertices)))
   for vert in mesh.vertices:
      file.write(binary_float(vert.co[0]))
      file.write(binary_float(vert.co[1]))
      file.write(binary_float(vert.co[2]))

   file.write(binary_u32(len(mesh.polygons)*3))
   for tri in mesh.polygons:
      file.write(binary_u32(tri.vertices[0]))
      file.write(binary_u32(tri.vertices[1]))
      file.write(binary_u32(tri.vertices[2]))

file.close()

t2 = time.clock()

total_time=t2-t0
file_time=t2-t1
other_time=t1-t0

print('Export time: ', total_time)
print('File time: ', file_time)
print('Other time: ', other_time)
