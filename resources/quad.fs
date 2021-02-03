#version 330 core
out vec3 color;

/*layout(binding = 0)*/uniform sampler2DRect front_tex;
/*layout(binding = 1)*/uniform sampler2DRect back_tex;
in vec2 tex_coord;

void main(){
   vec4 back_color = texture2DRect(back_tex, tex_coord);
   vec4 front_color = texture2DRect(front_tex, tex_coord);
   color = front_color.rgb + front_color.a * back_color.rgb;
}

