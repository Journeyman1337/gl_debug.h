<!--
SPDX-FileCopyrightText: 2021-2022 Daniel Aimé Valcour <fosssweeper@gmail.com>

SPDX-License-Identifier: MIT
-->

<!--
    Copyright (c) 2021-2022 Daniel Aimé Valcour

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in
    the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-->

 [![REUSE status](https://api.reuse.software/badge/git.fsfe.org/reuse/api)](https://api.reuse.software/info/git.fsfe.org/reuse/api)

# Header Only OpenGL Debuging Library

gl_debug.h is a library that helps with debugging OpenGL API calls in a powerful way. gl_debug.h works on the same thread as your OpenGL calls. When an error occurs, gl_debug.h will perform a debug break on the exact line where it occurred, making it as easy as possible to discover the source of the error. Also, unlike the OpenGL debug callback system, gl_debug.h works with OpenGL versions older than 4.3.

The debugging techniques used by gl_debug.h are performance heavy, but this library is designed so that when your application is compiled in release configuration, it will compile as if gl_debug.h was not there. gl_debug.h is only intended to be used during the development of an application.

Want to try the example project? You can set it up easily using bash console.

    git clone https://github.com/Journeyman-dev/gl_debug.h
    cd gl_debug.h
    cmake -S . -B ./build/ -D GLD_BUILD_EXAMPLE=ON -D GLD_EXAMPLE_AUTO_FETCH=ON
    cmake --build ./build/

