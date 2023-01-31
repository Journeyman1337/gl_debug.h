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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLD_CHECK_ON_RELEASE
#include <gld/gl_debug.h>
#include <stdio.h>

void myGldCallback(const char* message)
{
    printf("%s\n", message);
}

void testShaderCompile(const char* name, const char* vertex_source, const char* fragment_source)
{
    GLD_START();
    GLuint gl_program, gl_vertex_shader, gl_fragment_shader;
    GLD_CALL(gl_vertex_shader = glCreateShader(GL_VERTEX_SHADER));
    GLD_CALL(glShaderSource(gl_vertex_shader, 1, &vertex_source, NULL));
    GLD_COMPILE(gl_vertex_shader, name);
    GLD_CALL(gl_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER));
    GLD_CALL(glShaderSource(gl_fragment_shader, 1, &fragment_source, NULL));
    GLD_COMPILE(gl_fragment_shader, name);
    GLD_CALL(gl_program = glCreateProgram());
    GLD_CALL(glAttachShader(gl_program, gl_vertex_shader));
    GLD_CALL(glAttachShader(gl_program, gl_fragment_shader));
    GLD_LINK(gl_program, name);
    GLD_CALL(glDetachShader(gl_program, gl_vertex_shader));
    GLD_CALL(glDetachShader(gl_program, gl_fragment_shader));
    GLD_CALL(glDeleteShader(gl_vertex_shader));
    GLD_CALL(glDeleteShader(gl_fragment_shader));
    GLD_CALL(glDeleteProgram(gl_program));
}

int main()
{
    GLD_SET_CALLBACK(myGldCallback);
    if (!glfwInit())
    {
        printf("GLFW failed to initialize!\n");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(512, 512, "rlh test", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (!window)
    {
        printf("window failed to be created!\n");
        glfwTerminate();
        return 2;
    }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        window = NULL;
        glfwTerminate();
        printf("failed to load opengl bindings!\n");
        return 3;
    }
    GLD_CALL(glUseProgram(0));
    GLD_CALL(glUseProgram(1337));
    testShaderCompile("no errors",
        "#version 330 core\n"
        "layout (location = 0) in vec3 a_pos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
        "}\n",
        "#version 330 core\n"
        "out vec4 f_color;\n"
        "void main()\n"
        "{\n"
        "    f_color = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
        "}\n"
    );
    testShaderCompile("syntax errors",
        "#version 1234 base\n"
        "layout (location = 0) in vec32 a_pos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec3(a_pos.m, a_pos.n, a_pos.o, 1.0);\n"
        "}\n",
        "#version 330 core\n"
        "out vec4 f_color;\n"
        "void main()\n"
        "{\n"
        "    f_color = vec3(1.0f, 0.0f, 1.0f, 1.0f);\n"
        "}\n"
    );
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}