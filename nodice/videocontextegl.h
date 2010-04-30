/**
 * @file nodice/videocontextegl.h
 * @brief Public interface of the nodice/videocontextegl module.
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
#ifndef NODICE_VIDEOCONTEXTEGL_H
#define NODICE_VIDEOCONTEXTEGL_H 1

#include "nodice/videocontext.h"
#include "GLES/egl.h"


namespace NoDice
{
	class Config;


	class VideoContextEGL
	: public VideoContext
	{
	public:
		VideoContextEGL(const Config& config);

		void swapBuffers();

	private:
		EGLDisplay m_eglDisplay;
		EGLConfig  m_eglConfig;
		EGLContext m_eglContext;
		EGLSurface m_eglSurface;
	};

} // namespace NoDice

#endif // NODICE_VIDEOCONTEXTEGL_H
