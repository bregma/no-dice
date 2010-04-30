/**
 * @file nodice/videocontextsdl.cpp
 * @brief Implemntation of the nodice/videocontextsdl module.
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
#include "nodice/videocontextsdl.h"

#include <iostream>
#include "nodice/config.h"
#include <SDL/SDL.h>


NoDice::VideoContextSDL::VideoContextSDL(const Config& config)
{
	if (0 != ::SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		std::cerr << "*** ERRROR in SDL_InitSubSystem(SDL_INIT_VIDEO): " << ::SDL_GetError() << "\n";
		exit(1);
	}

	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	if (!videoInfo)
	{
		std::cerr << "*** ERRROR in SDL_GetVideoInfo(): " << ::SDL_GetError() << "\n";
		exit(1);
	}
	std::cerr << "==smw> video bpp = " << static_cast<int>(videoInfo->vfmt->BitsPerPixel) << "\n";

//	SDL_Rect** modes = SDL_ListModes(videoInfo->vfmt, SDL_OPENGL|SDL_FULLSCREEN);
	SDL_Rect** modes = SDL_ListModes(videoInfo->vfmt, SDL_OPENGL);
	if (modes == NULL)
	{
		std::cerr << "*** ERRROR no supported video modes available.\n";
		exit(1);
	}

	if (modes == (SDL_Rect **)-1)
	{
		std::cerr << "==smw> all video resolutions available.\n";
	}
	else
	{
		std::cerr << "==smw> Available Modes\n";
		for (int i=0; modes[i]; ++i)
		{
			std::cerr << "==smw> " << modes[i]->w << "x" << modes[i]->h << "\n";
		}
		if (modes[0])
		{
			m_width = modes[0]->w;
			m_height = modes[0]->h;
		}
	}
	std::cerr << "==smw> choosing full-screen mode " << m_width
		  << "x" << m_height
		  << "x" << static_cast<int>(videoInfo->vfmt->BitsPerPixel) << ".\n";

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	Uint32 videoFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_FULLSCREEN;
	Uint32 videoFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
	SDL_Surface* surface = SDL_SetVideoMode(m_width, m_height, videoInfo->vfmt->BitsPerPixel, videoFlags);
	if (!surface)
	{
		std::cerr << "*** ERRROR in SDL_SetVideoMode(): " << ::SDL_GetError() << "\n";
		exit(1);
	}
}

void NoDice::VideoContextSDL::swapBuffers()
{
	SDL_GL_SwapBuffers();
}


