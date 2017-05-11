#version 330 core
out vec3 color;

/*layout(binding = 0)*/uniform sampler2D front_tex;
/*layout(binding = 1)*/uniform sampler2D back_tex;
in vec2 tex_coord;

void main(){
   vec4 back_color = texture2D(back_tex, tex_coord);
   vec4 front_color = texture2D(front_tex, tex_coord);
   color = front_color.rgb + front_color.a * back_color.rgb;
}

