#version 330 core
layout(location = 0) in vec3 vert_pos;
layout(location = 1) in vec2 uv;
out vec2 tex_coord;

void main(){
   gl_Position = vec4(vert_pos, 1.0);
   tex_coord = uv;
}

