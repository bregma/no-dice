/**
 * @file nodice/video.cpp
 * @brief Implemntation of the nodice/video module.
 *
 * Copyright 2009 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/video.h"

#include <iostream>
#ifdef HAVE_EGL
# include "nodice/videocontextegl.h"
#else
# include "nodice/videocontextsdl.h"
#endif

namespace
{
	void initGL()
	{
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

NoDice::Video::Video(const Config& config)
#ifdef HAVE_EGL
: m_context(new VideoContextEGL(config))
#else
: m_context(new VideoContextSDL(config))
#endif
, m_screenWidth(640)
, m_screenHeight(480)
{
	initGL();
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


NoDice::Video::~Video()
{
}


void NoDice::Video::update()
{
	m_context->swapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.0f);
}



