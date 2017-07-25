/**
 * @file nodice/video.cpp
 * @brief Implemntation of the nodice/video module.
 */
/*
 * Copyright 2009,2010,2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
 *
 * This file is part of no-dice.
 *
 * No-dice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * No-dice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with no-dice.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "nodice/video.h"

#include <iostream>
#include "nodice/config.h"
#ifdef HAVE_EGL
# include "nodice/videocontextegl.h"
#else
# include "nodice/videocontextsdl.h"
#endif

namespace
{
  void initGL()
  {
    initGlVboExtension();

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
#ifdef HAVE_OPENGL_ES
    glClearDepthf(1.0f);
#else
    glClearDepth(1.0f);
#endif
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_EQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  }
}

NoDice::Video::
Video(Config const* config)
#ifdef HAVE_EGL
: m_context(new VideoContextEGL(*config))
#else
: m_context(new VideoContextSDL(config))
#endif
{
  initGL();
  glViewport(0, 0, config->screen_width(), config->screen_height());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


NoDice::Video::
~Video()
{
}


void NoDice::Video::
update()
{
  m_context->swapBuffers();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -1.0f);
}



