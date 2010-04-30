/**
 * @file nodice/videocontextegl.cpp
 * @brief Implemntation of the nodice/videocontextegl module.
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
#include "nodice/videocontextegl.h"

#include <iostream>
#include "nodice/config.h"
#include <stdexcept>

extern "C" void* updater_get_native_window();


NoDice::VideoContextEGL::VideoContextEGL(const Config& config)
{
	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (m_eglDisplay == EGL_NO_DISPLAY)
	{
		std::cerr << "error " << eglGetError() << "\n";
		throw std::runtime_error("error in eglGetDisplay");
	}

	EGLint majorVersion;
	EGLint minorVersion;
	EGLBoolean estat = eglInitialize(m_eglDisplay, &majorVersion, &minorVersion);
	if (estat == EGL_FALSE)
	{
		std::cerr << "error " << eglGetError() << "\n";
		throw std::runtime_error("error in eglInitialize");
	}

	EGLint eglConfigAttributes[] =
	{
		EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
		EGL_ALPHA_SIZE, 8,
		EGL_NONE
	};

	EGLint numConfigs;
	const EGLint maxConfigs = 4;
	EGLConfig eglConfig[maxConfigs];
	estat = eglChooseConfig(m_eglDisplay, eglConfigAttributes, eglConfig, maxConfigs, &numConfigs);
	if (estat == EGL_FALSE)
	{
		std::cerr << "error " << eglGetError() << "\n";
		throw std::runtime_error("error in eglGetConfigs");
	}
	std::cerr << "==smw> num EGL configs = " << numConfigs << "\n";

	for (EGLint i = 0; i < numConfigs; ++i)
	{
		EGLint red;
		eglGetConfigAttrib(m_eglDisplay, eglConfig[i], EGL_RED_SIZE, &red);
		EGLint green;
		eglGetConfigAttrib(m_eglDisplay, eglConfig[i], EGL_GREEN_SIZE, &green);
		EGLint blue;
		eglGetConfigAttrib(m_eglDisplay, eglConfig[i], EGL_BLUE_SIZE, &blue);
		EGLint alpha;
		eglGetConfigAttrib(m_eglDisplay, eglConfig[i], EGL_ALPHA_SIZE, &alpha);
		std::cerr << "==smw> config(" << i << ") red=" << red << " green=" << green << " blue=" << blue << " alpha=" << alpha << "\n";
	}

	EGLint eglSurfaceAttributes[] =
	{
		EGL_NONE
	};

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig[0], updater_get_native_window(), eglSurfaceAttributes);
	if (m_eglSurface == EGL_NO_SURFACE)
	{
		std::cerr << "error " << eglGetError() << "\n";
		throw std::runtime_error("error in eglCreateWindowSurface");
	}

	static const EGLint glContextAttribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	m_eglContext = eglCreateContext(m_eglDisplay, eglConfig[0], EGL_NO_CONTEXT, glContextAttribs);
	if (m_eglContext == EGL_NO_CONTEXT)
	{
		std::cerr << "error " << eglGetError() << "\n";
		throw std::runtime_error("error in eglCreateContext");
	}

	estat = eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
	if (estat == EGL_FALSE)
	{
		std::cerr << "error " << eglGetError() << "\n";
		throw std::runtime_error("error in eglMakeCurrent");
	}
}

void NoDice::VideoContextEGL::swapBuffers()
{
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}


