/**
 * @file nodice/opengl.cpp
 * @brief Implemntation of the no-dice opengl module.
 *
 * Copyright 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of Version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "nodice/opengl.h"

#include <cstdlib>
#include <cstring>

using namespace std;

void initGlVboExtension()
{
#ifndef HAVE_GL_VERTEX_BUFFERS
  int found = 0;
  const char* extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
  char* terminator = NULL;
  for(;;)
  {
    char* where = (char*)strstr(extensions, "GL_ARB_vertex_buffer_object");
    if (!where) break;
    terminator = where + strlen("GL_ARB_vertex_buffer_object");
    if (where == extensions || *(where - 1) == ' ')
    {
      if (*terminator == ' ' || *terminator == '\0')
      {
	found = 1;
	break;
      }
    }
    extensions = terminator;
  }

  if (!found)
  {
		std::cerr << "OpenGL vertex buffer extension is not supported.\n";
	  exit(1);

  }
 
  glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffersARB");
  glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffersARB");
  glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBufferARB");
  glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferDataARB");
  /* glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubDataARB"); */
#endif
}

void check_gl_error(const std::string& msg)
{
	GLenum err = glGetError();
	while (err != 0)
	{
		std::cerr << "GL error 0x" << std::hex << err << std::dec << " at " << msg << "\n";
		err = glGetError();
	}
}

