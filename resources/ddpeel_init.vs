#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform unsigned int time;
uniform mat4 projection_matrix;

void main(){
   float theta = time * 0.002;
   mat3 rotation_matrix = mat3(
         cos(theta), 0, sin(theta),
         0, 1, 0,
         -sin(theta), 0, cos(theta)
   );
   vec3 position = vertexPosition_modelspace.xzy*vec3(0.8, 0.8, -0.8);
   //position += vec3(cos(theta)*0.6, sin(theta)*0.6, -0.9);
   position *= rotation_matrix;
   position += vec3(0.0, 0.0, -0.9);
   gl_Position = vec4(position, 1.0) * projection_matrix;
}

