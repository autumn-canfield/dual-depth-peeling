#version 330 core
layout(location = 0) out vec2 depth;
layout(location = 1) out vec4 front_color;
layout(location = 2) out vec4 back_color;

//layout(binding = 0)uniform sampler2DRect depth_tex;
uniform sampler2DRect depth_tex;

in vec3 pos;

#define bias 0.000001

void main (){
   vec2 prev_depth = texture2DRect(depth_tex, gl_FragCoord.xy).xy;
   float front_depth = -prev_depth.x;
   float back_depth = prev_depth.y;
   float cur_depth = gl_FragCoord.z;

   vec4 color = vec4(((sin(pos.x*35))/4.0+0.5)*(pos+0.5), 0.5);

   front_color = vec4(0.0, 0.0, 0.0, 0.0);
   back_color = vec4(0.0, 0.0, 0.0, 0.0);
   depth = vec2(-1.0, -1.0);

   // Skip fragment
   if(cur_depth + bias < front_depth || cur_depth - bias > back_depth)
   {
      discard;
   }

   // Peel fragment
   if(cur_depth - bias > front_depth && cur_depth + bias < back_depth)
   {
      depth = vec2(-gl_FragCoord.z, gl_FragCoord.z);
      return;
   }

   // Shade fragment
   if(cur_depth < front_depth + bias && cur_depth > front_depth - bias)
   {
      front_color = color;
   }else
   {
      back_color = color;
   }
}

