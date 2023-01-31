// SPDX-FileCopyrightText: 2021-2023 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

/*
    MIT License

    Copyright (c) 2021-2023 Daniel Valcour

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/* gl_debug.h v1.0.0
   MIT Licensed OpenGL Debuging Library
   The source for this library can be found on GitHub:
   https://github.com/Journeyman-dev/gl_debug.h

   WHY USE THIS LIBRARY?
   gl_debug.h is a library that helps with debugging OpenGL API calls in a powerful way. gl_debug.h
   works on the same thread as your OpenGL calls. When an error occurs, gl_debug.h will perform a
   debug break on the exact line where it occured, making it as easy as possible to discover the
   source of the error. Also, unlike the OpenGL debug callback system, gl_debug.h works with OpenGL
   versions older than 4.3.

   The debugging techniques used by gl_debug.h are performance heavy, but this library is designed
   so that when your application is compiled in release configuration, it will compile as if
   gl_debug.h was not there. gl_debug.h is only intended to be used during the development of an
   application.

    HOW TO SETUP
    The gl_debug.h library can be included in your project in one of three different ways:
        - Copy and paste the gl_debug.h file directly into your source tree.
        - Clone the GitHub as a git submodule to your project's repository.
          1. In bash console from the root directory of your project's repository:
                git submodule add https://github.com/Journeyman-dev/gl_debug.h
                git submodule update --init
          2. In your project's top level CMakeLists.txt:
                set(GLD_BUILD_EXAMPLE OFF) # Set to ON to build the example target.
                set(GLD_EXAMPLE_AUTO_FETCH OFF) # Set to ON to fetch the example dependencies (see bellow).
                add_subdirectory(gl_debug.h)
          3. Link it with the CMake target of your project:
               target_link_libraries(YOUR_TARGET_NAME PRIVATE gld::gld)
        - Fetch it using CMake FetchContent:
          1. Fetch it in your project's CMakeLists.txt:
                Include(FetchContent)
                FetchContent_Declare(
                        gld
                        GIT_REPOSITORY https://github.com/Journeyman-dev/gl_debug.h
                        GIT_TAG        v1.0.0 # replace with the version that you want
                        FIND_PACKAGE_ARGS
                )
                set(GLD_BUILD_EXAMPLE OFF) # Set to ON to build the example target.
                set(GLD_EXAMPLE_AUTO_FETCH OFF) # Set to ON to fetch the example dependencies (see bellow).
                FetchContent_MakeAvailable(gld)
          2. Link it with the CMake target ofyour project:
                target_link_libraries(YOUR_TARGET_NAME PRIVATE gld::gld)

   After gl_debug.h is included in your project, you must implement the library before you can
   actually use it. To implement gl_debug.h, create a new .c or .cpp file and write in it the
   following:

                #include <glad/glad.h> // you can use a different opengl loader here (see bellow)
                #define GLD_CHECK_ON_RELEASE // optional (see bellow)
                #define GLD_IMPLEMENTATION
                #include <gld/gl_debug.h>

   gl_debug.h requires you to load bindings to its functions before it is implemented. There are many
   loading libraries avaliable, including glad and GLEW. gl_debug.h will work no matter what loading
   library you choose to use, but be sure to include it before you implement the library as shown
   above.

   If you want to check for shader compile errors or program link errors even when your program is
   compiled in release configuration, you can define the macro GLD_CHECK_ON_RELEASE before
   implementing gl_debug.h. If you do this, you must also define the macro again in every file you
   wish to check for compile and linking errors on release.

   HOW TO USE
   gl_debug.h calls are all in the form of macros. This is important so that the compiler can ignore
   them when your project is in release configuration.

   The first macro you should call in an application that uses gl_debug.h is GLD_SET_CALLBACK. Pass
   into this macro a function pointer to a function that takes a const char* arguments. When gl_debug.h
   comes across an error, it will pass a log message explaining the error into this callback. The message
   will include the line number and file name where the error occured, making it very easy locate.

   To use gl_debug.h in a block of OpenGL code, the first thing you should do is call the GLD_START()
   macro. This will clear the OpenGL error buffer, making sure that any OpenGL errors that occured
   in other blocks of code that were not checked will not cause gl_debug.h to mistakenly think an
   error has occured in the block bellow.

   Each OpenGL call in the block should be passed into the GLD_CALL() macro. This will cause
   gl_debug.h to check if any errors occured after that call is done executing. Since gl_debug.h uses
   macros and not functions, you can pass in an entire programming statement into the macro to debug
   it. For example, this is valid syntax:

                GLD_CALL(GLuint my_program = glCreateProgram());

   There are other macros made for specific debugging situations. GLD_COMPILE can be used to compile
   a shader and check for compilation errors, passing them into the callback. GLD_LINK can be used
   to do the same with shader program linking. Both of these functions take in an identifier string
   as a second argumet, which is passed to the log message callback along with any GPU driver
   generated description of the error. For example:

                GLD_COMPILE(my_fragment_shader, "my fragment shader");
                GLD_LINK(my_shader_program, "my shader program");

   It is common to want to be able to debug these actions even when your application is released.
   If you want this, you must define GLD_CHECK_ON_RELEASE before the implementation and every
   other include of gl_debug.h. This was explained above in the HOW TO SETUP section.

  CHANGELOG
    - Version 1.0
        - Initial Release.
*/

#ifdef __cplusplus  // extern C guard
extern "C"
{
#endif

#ifndef GLD_H
#  define GLD_H
#endif

#if !defined(NDEBUG)
#  ifndef GLD_PROTOTYPES
#    define GLD_PROTOTYPES
  typedef enum gldbool_t
  {
    GLD_FALSE = 0,
    GLD_TRUE = 1
  } gldbool_t;
  void gldLog(const char* message);
  void gldClearErrors();
  void gldSetCallback(void (*callback)(const char*));
  gldbool_t gldErrorCheck(const char* glFunc, int line, const char* file);
  gldbool_t gldCheckShaderCompile(unsigned int handle, const char* identifier);
  gldbool_t gldCheckProgramLink(unsigned int handle, const char* identifier);
  void gldLogAllErrors(const char* func_name);
#  endif
#  define GLD_START() gldClearErrors();
#  define GLD_SET_CALLBACK(callback) gldSetCallback(callback);
#  define GLD_CALL(glFunc)           \
    glFunc;                          \
    gldErrorCheck(#glFunc, __LINE__, __FILE__);
#  define GLD_COMPILE(shader_handle, shader_name)               \
    glCompileShader(shader_handle);                             \
    gldCheckShaderCompile(shader_handle, shader_name);
#  define GLD_LINK(program_handle, program_name)                \
    glLinkProgram(program_handle);                              \
    gldCheckProgramLink(program_handle, program_name);
#elif defined(GLD_CHECK_ON_RELEASE)  // && defined(NDEBUG)
#  define GLD_START()
#  define GLD_SET_CALLBACK(callback) gldSetCallback(callback);
#  define GLD_CALL() glFunc;
#  define GLD_COMPILE(shader_handle, shader_name) \
    glCompileShader(shader_handle);               \
    gldCheckShaderCompile(shader_handle, shader_name);
#  define GLD_LINK(program_handle, program_name) \
    glLinkProgram(program_handle);               \
    gldCheckProgramLink(program_handle, program_name);
#else  //! defined(GLD_CHECK_ON_RELEASE) && defined(NDEBUG)
#  define GLD_START()
#  define GLD_SET_CALLBACK(callback)
#  define GLD_CALL() glFunc;
#  define GLD_COMPILE(shader_handle, shader_name) glCompileShader(shader_handle);
#  define GLD_LINK(program_handle, program_name) glLinkProgram(program_handle);
#endif

#if defined(GLD_IMPLEMENTATION) && !defined(GLD_IMPLEMENTED)
#  define GLD_IMPLEMENTED
#  if !defined(NDEBUG) || defined(GLD_CHECK_ON_RELEASE)
#    define GL_INVALID_ENUM 0x0500
#    define GL_INVALID_VALUE 0x0501
#    define GL_INVALID_OPERATION 0x0502
#    define GL_STACK_OVERFLOW 0x0503
#    define GL_STACK_UNDERFLOW 0x0504
#    define GL_OUT_OF_MEMORY 0x0505
#    define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#    define GL_CONTEXT_LOST 0x0507
#    define GL_TABLE_TOO_LARGE 0x8031
  static void (*sGLD_CALLBACK)(const char*) = 0;

#include <stdio.h>

  void gldSetCallback(void (*callback)(const char*))
  {
    sGLD_CALLBACK = callback;
  }

  void gldLog(const char* message)
  {
    if (!sGLD_CALLBACK)
    {
      return;
    }
    sGLD_CALLBACK(message);
  }

  gldbool_t gldCheckShaderCompile(unsigned int handle, const char* identifier)
  {
    GLint compiled;
    GLD_CALL(glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled));
    if (compiled != GL_TRUE)
    {
      GLsizei log_length = 0;
      GLchar info_log[1024];
      GLD_CALL(glGetShaderInfoLog(handle, 1024, &log_length, info_log))
      if (log_length < 1024)
      {
        info_log[log_length] = '\0';
      }
      else
      {
        info_log[log_length - 1] = '\0';
      }
      char message[1024];
      snprintf(message, sizeof(message), "shader compile error for program \"%s\":\n%s", identifier, info_log);
      gldLog(message);
      return GLD_TRUE;
    }
    return GLD_FALSE;
  }

  gldbool_t gldCheckProgramLink(unsigned int handle, const char* identifier)
  {
    GLint program_linked;
    GLD_CALL(glGetProgramiv(handle, GL_LINK_STATUS, &program_linked));
    if (program_linked != GL_TRUE)
    {
      GLsizei log_length = 0;
      GLchar info_log[1024];
      GLD_CALL(glGetProgramInfoLog(handle, 1024, &log_length, info_log));
      if (log_length < 1024)
      {
        info_log[log_length] = '\0';
      }
      else
      {
        info_log[log_length - 1] = '\0';
      }
      char message[1024];
      snprintf(message, sizeof(message), "shader link error for program \"%s\":\n%s", identifier, info_log);
      gldLog(message);
      return GLD_TRUE;
    }
    return GLD_FALSE;
  }
#  endif
#  if !defined(NDEBUG)
  void gldClearErrors()
  {
    while (glGetError() != GL_NO_ERROR);
  }

  void gldErrorLog(const char* error, const char* statement, int line, const char* file)
  {
    char message[1024];
    snprintf(message, sizeof(message), "\"%s\" from statement \"%s\" at line \"%d\" in file \"%s\"", error, statement, line, file);
    gldLog(message);
  }

  gldbool_t gldErrorCheck(const char* statement, int line, const char* file)
  {
    GLenum err = glGetError();
    switch (err)
    {
    case GL_NO_ERROR:
      return GLD_FALSE;
    case GL_INVALID_ENUM:
      gldErrorLog("GL_INVALID_ENUM", statement, line, file);
      return GLD_TRUE;
    case GL_INVALID_VALUE:
      gldErrorLog("GL_INVALID_VALUE", statement, line, file);
      return GLD_TRUE;
    case GL_INVALID_OPERATION:
      gldErrorLog("GL_INVALID_OPERATION", statement, line, file);
      break;
    case GL_STACK_OVERFLOW:
      gldErrorLog("GL_STACK_OVERFLOW", statement, line, file);
      return GLD_TRUE;
    case GL_OUT_OF_MEMORY:
      gldErrorLog("GL_OUT_OF_MEMORY", statement, line, file);
      return GLD_TRUE;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      gldErrorLog("GL_INVALID_FRAMEBUFFER_OPERATION", statement, line, file);
      return GLD_TRUE;
    case GL_CONTEXT_LOST:
      gldErrorLog("GL_CONTEXT_LOST", statement, line, file);
      return GLD_TRUE;
    case GL_TABLE_TOO_LARGE:
      gldErrorLog("GL_TABLE_TOO_LARGE", statement, line, file);
      return GLD_TRUE;
    default:
      gldErrorLog("unknown error", statement, line, file);
      return GLD_TRUE;
    }
  }
#  endif
#endif

#ifdef __cplusplus  // extern C guard
}
#endif