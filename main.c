#include<stdio.h>
#define GLEW_STATIC
#include<GL/glew.h>

#include"types.h"
#include"smallwin/smallwin.h"
#include"shaders.h"
#include"loading/load_mesh.h"
#include"error.h"

u32 width = 1024, height = 800;

u32 num_vertices, num_indices;
f32 *vertex_data;
u32 *index_data;
u32 vertex_buffer, index_buffer;
const f32 quad_vb_data[18] = {
   -1.0, -1.0, 0.0,
   1.0, -1.0, 0.0,
   -1.0,  1.0, 0.0,
   -1.0,  1.0, 0.0,
   1.0, -1.0, 0.0,
   1.0,  1.0, 0.0
};
const f32 quad_uv_data[12] = {
   0.0, 0.0,
   1.0, 0.0,
   0.0, 1.0,
   0.0, 1.0,
   1.0, 0.0,
   1.0, 1.0
};
u32 quad_vb, quad_uv;

void setup_meshes()
{
   loaded_mesh mesh = load_mesh("resources/dragon.obj");
   num_vertices = mesh.num_vertices;
   num_indices = mesh.num_indices;
   vertex_data = mesh.vertices;
   index_data = mesh.indices;

   glGenBuffers(1, &vertex_buffer); gl_error_check("At line: %u\n\n", __LINE__);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); gl_error_check("At line: %u\n\n", __LINE__);
   glBufferData(GL_ARRAY_BUFFER, num_vertices*3*sizeof(f32), vertex_data,
         GL_STATIC_DRAW); gl_error_check("At line: %u\n\n", __LINE__);

   glGenBuffers(1, &index_buffer); gl_error_check("At line: %u\n\n", __LINE__);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); gl_error_check("At line: %u\n\n", __LINE__);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices*sizeof(u32), index_data,
         GL_STATIC_DRAW); gl_error_check("At line: %u\n\n", __LINE__);

   glGenBuffers(1, &quad_vb); gl_error_check("At line: %u\n\n", __LINE__);
   glBindBuffer(GL_ARRAY_BUFFER, quad_vb); gl_error_check("At line: %u\n\n", __LINE__);
   glBufferData(GL_ARRAY_BUFFER, 18*sizeof(f32), quad_vb_data, GL_STATIC_DRAW); gl_error_check("At line: %u\n\n", __LINE__);

   glGenBuffers(1, &quad_uv); gl_error_check("At line: %u\n\n", __LINE__);
   glBindBuffer(GL_ARRAY_BUFFER, quad_uv); gl_error_check("At line: %u\n\n", __LINE__);
   glBufferData(GL_ARRAY_BUFFER, 12*sizeof(f32), quad_uv_data, GL_STATIC_DRAW); gl_error_check("At line: %u\n\n", __LINE__);
}


u32 dragon_vs, dragon_fs, dragon_program;
u32 quad_vs, quad_fs, quad_program;
u32 ddpeel_init_vs, ddpeel_init_fs, ddpeel_init_program;

void setup_shaders()
{
   dragon_vs = load_vertex_shader("resources/dragon.vs");
   dragon_fs = load_fragment_shader("resources/dragon.fs");
   dragon_program = link_program(dragon_vs, dragon_fs);

   quad_vs = load_vertex_shader("resources/quad.vs");
   quad_fs = load_fragment_shader("resources/quad.fs");
   quad_program = link_program(quad_vs, quad_fs);

   ddpeel_init_vs = load_vertex_shader("resources/ddpeel_init.vs");
   ddpeel_init_fs = load_fragment_shader("resources/ddpeel_init.fs");
   ddpeel_init_program = link_program(ddpeel_init_vs, ddpeel_init_fs);
}


u32 framebuffer[2];
u32 front_tex;
u32 back_tex;
u32 depth_tex[2];
u32 draw_buffers[3] = {
   GL_COLOR_ATTACHMENT0,
   GL_COLOR_ATTACHMENT1,
   GL_COLOR_ATTACHMENT2
};

void setup_framebuffers()
{
   glGenFramebuffers(2, framebuffer); gl_error_check("At line: %u\n\n", __LINE__);
   glGenTextures(2, depth_tex); gl_error_check("At line: %u\n\n", __LINE__);
   glGenTextures(1, &front_tex); gl_error_check("At line: %u\n\n", __LINE__);
   glGenTextures(1, &back_tex); gl_error_check("At line: %u\n\n", __LINE__);

   u32 i;
   for(i=0; i<2; i++)
   {
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);

      glBindTexture(GL_TEXTURE_RECTANGLE, depth_tex[i]); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RG32F, width, height, 0,
            GL_RG, GL_FLOAT, 0); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            depth_tex[i], 0); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);

      glBindTexture(GL_TEXTURE_2D, front_tex); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
            front_tex, 0); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);

      glBindTexture(GL_TEXTURE_2D, back_tex); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, 0); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
            back_tex, 0); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);

      glDrawBuffers(3, draw_buffers); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);

      glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glBlendFuncSeparatei(1, GL_DST_ALPHA, GL_ONE, GL_ZERO,
            GL_ONE_MINUS_SRC_ALPHA); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glBlendFuncSeparatei(2, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
            GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glBlendEquationi(0, GL_MAX); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glBlendEquationi(1, GL_FUNC_ADD); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);
      glBlendEquationi(2, GL_FUNC_ADD); gl_error_check("At line: %u, i: %u\n\n", __LINE__, i);

      if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
         error("Imcomplete framebuffer! (i=%u)\n", i);
   }

}

window mainwin;

void key_down_handler(u8 key)
{
   if(key == 27)
      exit(0);
}

int main()
{
   mainwin = smallwin_open_window("Dual Depth Peeling Demo", width, height, 3, 3);

   smallwin_keydown_callback = &key_down_handler;

   glewExperimental = GL_TRUE;
   if(glewInit() != GLEW_OK)
   {
      smallwin_error_message((const char *)"Error initializing GLEW!");
   }
   
#ifdef DEVELOP
   // GLEW uses glGetString instead of glGetStringi which causesGL_INVALID_ENUM.
   // This is so we don't mistakenly think our code is responsible for the error.
   glGetError();
#endif

   u32 vertex_array_id;
   glGenVertexArrays(1, &vertex_array_id); gl_error_check("At line: %u\n\n", __LINE__);
   glBindVertexArray(vertex_array_id); gl_error_check("At line: %u\n\n", __LINE__);

   setup_meshes();
   setup_shaders();
   setup_framebuffers();

   glFrontFace(GL_CCW); gl_error_check("At line: %u\n\n", __LINE__);
   glCullFace(GL_BACK); gl_error_check("At line: %u\n\n", __LINE__);
   glDisable(GL_CULL_FACE); gl_error_check("At line: %u\n\n", __LINE__);
   glEnable(GL_BLEND); gl_error_check("At line: %u\n\n", __LINE__);
   glEnable(GL_DEPTH_TEST); gl_error_check("At line: %u\n\n", __LINE__);
   glDepthFunc(GL_LESS); gl_error_check("At line: %u\n\n", __LINE__);

   f32 camera_near = 0.3;
   f32 camera_far = 5.99;
   f32 right = 0.4;
   f32 left  = -0.4;
   f32 bottom = -0.4;
   f32 top = 0.4;


   f32 projection_matrix[16] =
   {
      (2.0*camera_near)/(right-left), 0.0, 0.0, 0.0,
      0.0, (2.0*camera_near)/(top-bottom), 0.0, 0.0,
      0.0, 0.0, -(camera_far+camera_near)/(camera_far-camera_near), -(2.0*camera_near*camera_far)/(camera_far-camera_near),
      0.0, 0.0, -1.0, 0.0
   };


   u64 frame = 0;

   while(1)
   {
      smallwin_swap_buffers(mainwin);
      if(!non_blocking_message_loop())
         break;

      glEnableVertexAttribArray(0); gl_error_check("At line: %u\n\n", __LINE__);


      const u32 front_clear[4] = {0, 0, 0, 1};
      const u32 back_clear[4] = {0, 0, 0, 0};
      const f32 depth_clear[4] = {-1.0, -1.0, -1.0, -1.0};

      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]); gl_error_check("At line: %u\n\n", __LINE__);
      glClearBufferfv(GL_COLOR, 0, depth_clear); gl_error_check("At line: %u\n\n", __LINE__);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[1]); gl_error_check("At line: %u\n\n", __LINE__);
      glClearBufferfv(GL_COLOR, 0, depth_clear); gl_error_check("At line: %u\n\n", __LINE__);
      glClearBufferuiv(GL_COLOR, 1, front_clear); gl_error_check("At line: %u\n\n", __LINE__);
      glClearBufferuiv(GL_COLOR, 2, back_clear); gl_error_check("At line: %u\n\n", __LINE__);

      glUseProgram(ddpeel_init_program); gl_error_check("At line: %u\n\n", __LINE__);
      glUniform1i(glGetUniformLocation(ddpeel_init_program, "time"), frame); gl_error_check("At line: %u\n\n", __LINE__);
      glUniformMatrix4fv(glGetUniformLocation(ddpeel_init_program, "projection_matrix"),
            1, GL_FALSE, projection_matrix); gl_error_check("At line: %u\n\n", __LINE__);
      glDrawBuffers(1, draw_buffers); gl_error_check("At line: %u\n\n", __LINE__);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); gl_error_check("At line: %u\n\n", __LINE__);
      glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, (void*)0); gl_error_check("At line: %u\n\n", __LINE__);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); gl_error_check("At line: %u\n\n", __LINE__);
      glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*)0); gl_error_check("At line: %u\n\n", __LINE__);
      glDrawBuffers(3, draw_buffers); gl_error_check("At line: %u\n\n", __LINE__);

      glUseProgram(dragon_program); gl_error_check("At line: %u\n\n", __LINE__);
      glUniform1i(glGetUniformLocation(dragon_program, "time"), frame); gl_error_check("At line: %u\n\n", __LINE__);
      glUniform1i(glGetUniformLocation(dragon_program, "depth_tex"), 0); gl_error_check("At line: %u\n\n", __LINE__);
      glUniformMatrix4fv(glGetUniformLocation(dragon_program, "projection_matrix"),
            1, GL_FALSE, projection_matrix); gl_error_check("At line: %u\n\n", __LINE__);

      glBindTexture(GL_TEXTURE_2D, 0); gl_error_check("At line: %u\n\n", __LINE__);
      
      u32 layer;
      for(layer=0; layer<3; layer++)
      {
         glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[layer%2]); gl_error_check("At line: %u\n\n", __LINE__);
         glClearBufferfv(GL_COLOR, 0, depth_clear); gl_error_check("At line: %u\n\n", __LINE__);
         glActiveTexture(GL_TEXTURE0); gl_error_check("At line: %u\n\n", __LINE__);
         glBindTexture(GL_TEXTURE_RECTANGLE, depth_tex[(layer+1)%2]); gl_error_check("At line: %u\n\n", __LINE__);
         glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void*)0); gl_error_check("At line: %u\n\n", __LINE__);
      }

      glBindTexture(GL_TEXTURE_2D, 0); gl_error_check("At line: %u\n\n", __LINE__);

      glBindFramebuffer(GL_FRAMEBUFFER, 0); gl_error_check("At line: %u\n\n", __LINE__);
      glClearColor(0.0, 0.0, 0.0, 1.0); gl_error_check("At line: %u\n\n", __LINE__);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); gl_error_check("At line: %u\n\n", __LINE__);

      glUseProgram(quad_program); gl_error_check("At line: %u\n\n", __LINE__);
      glActiveTexture(GL_TEXTURE0); gl_error_check("At line: %u\n\n", __LINE__);
      glBindTexture(GL_TEXTURE_2D, front_tex); gl_error_check("At line: %u\n\n", __LINE__);
      glActiveTexture(GL_TEXTURE1); gl_error_check("At line: %u\n\n", __LINE__);
      glBindTexture(GL_TEXTURE_2D, back_tex); gl_error_check("At line: %u\n\n", __LINE__);
      glUniform1i(glGetUniformLocation(quad_program, "front_tex"), 0); gl_error_check("At line: %u\n\n", __LINE__);
      glUniform1i(glGetUniformLocation(quad_program, "back_tex"), 1); gl_error_check("At line: %u\n\n", __LINE__);

      glEnableVertexAttribArray(1); gl_error_check("At line: %u\n\n", __LINE__);
      glBindBuffer(GL_ARRAY_BUFFER, quad_uv); gl_error_check("At line: %u\n\n", __LINE__);
      glVertexAttribPointer(1, 2, GL_FLOAT, 0, 0, (void*)0); gl_error_check("At line: %u\n\n", __LINE__);
      glBindBuffer(GL_ARRAY_BUFFER, quad_vb); gl_error_check("At line: %u\n\n", __LINE__);
      glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, (void*)0); gl_error_check("At line: %u\n\n", __LINE__);
      glDrawArrays(GL_TRIANGLES, 0, 18); gl_error_check("At line: %u\n\n", __LINE__);
      glDisableVertexAttribArray(1); gl_error_check("At line: %u\n\n", __LINE__);
      glDisableVertexAttribArray(0); gl_error_check("At line: %u\n\n", __LINE__);

      frame++;
   }

   glDeleteShader(dragon_vs); gl_error_check("At line: %u\n\n", __LINE__);
   glDeleteShader(dragon_fs); gl_error_check("At line: %u\n\n", __LINE__);
   glDeleteShader(quad_vs); gl_error_check("At line: %u\n\n", __LINE__);
   glDeleteShader(quad_fs); gl_error_check("At line: %u\n\n", __LINE__);
   glDeleteShader(ddpeel_init_vs); gl_error_check("At line: %u\n\n", __LINE__);
   glDeleteShader(ddpeel_init_fs); gl_error_check("At line: %u\n\n", __LINE__);

   exit_smallwin();

   return 0;
}
