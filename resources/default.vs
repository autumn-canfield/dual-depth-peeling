#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
void main(){
   vec3 pos = vertexPosition_modelspace;
   gl_Position = vec4(pos, 1.0);
}

