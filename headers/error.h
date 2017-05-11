#pragma once
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#define GLEW_STATIC
#include <GL/glew.h>

static inline void error(const char *fmt, ...)
{
   va_list args;
   va_start(args, fmt);
   vprintf(fmt, args);
   exit(1);
}

static inline void gl_error_check(const char *fmt, ...)
{
   u32 error_num = glGetError();
   if(error_num != GL_NO_ERROR)
   {
      va_list args;
      va_start(args, fmt);
      vprintf(fmt, args);
      switch(error_num)
      {
         case GL_INVALID_ENUM:
            printf("Error: GL_INVALID_ENUM\n");
            break;
         case GL_INVALID_VALUE:
            printf("Error: GL_INVALID_VALUE\n");
            break;
         case GL_INVALID_OPERATION:
            printf("Error: GL_INVALID_OPERATION\n");
            break;
         case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("Error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
            break;
         case GL_OUT_OF_MEMORY:
            printf("Error: GL_OUT_OF_MEMORY\n");
            break;
         case GL_STACK_UNDERFLOW:
            printf("Error: GL_STACK_UNDERFLOW\n");
            break;
         case GL_STACK_OVERFLOW:
            printf("Error: GL_STACK_OVERFLOW\n");
            break;
         default:
            printf("Error: glGetError() = %u\n", error_num);
            break;
      }
   }
}

