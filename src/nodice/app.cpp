/**
 * @file nodice/app.cpp
 * @brief Implemntation of the nodice/app module.
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
#include "nodice/app.h"

#include <cstdlib>
#include <iostream>
#include "nodice/config.h"
#include "nodice/font.h"
#include "nodice/fontcache.h"
#include <SDL/SDL.h>

#ifndef DATA_DIR
# define DATA_DIR
#endif

NoDice::App::SdlInit::SdlInit()
{
	Uint32 flags = SDL_INIT_NOPARACHUTE;
	int istat = ::SDL_Init(flags);
	if (istat < 0)
	{
		std::cerr << "*** ERRROR in SDL_Init: " << ::SDL_GetError() << "\n";
		exit(1);
	}

	std::atexit(::SDL_Quit);
}


NoDice::App::SdlInit::~SdlInit()
{
	::SDL_Quit();
}

/**
 * The App object takes care of initializing the even system.
 */
NoDice::App::App(const NoDice::Config& config)
: m_sdlInit()
, m_video(config)
{
}


NoDice::App::~App()
{
}

int NoDice::App::run()
{
	bool done = false;
	bool isActive = true;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_ACTIVEEVENT:
				std::cerr << "==smw> SDL_ACTIVEEVENT event type received, gain=" << (int)event.active.gain << ".\n";
				isActive = (event.active.gain != 0);
				break;
			case SDL_KEYDOWN:
				std::cerr << "==smw> SDL_KEYDOWN event type received.\n";
				done = true;
				break;
			case SDL_QUIT:
				std::cerr << "==smw> SDL_QUIT event type received.\n";
				done = true;
				break;
			default:
				std::cerr << "==smw> event type " << static_cast<int>(event.type) << " received.\n";
				break;
			}
		}

		if (isActive)
		{
			Font& font = FontCache::get("FreeSans", 18);
			glColor3ub(0, 0, 0xff);
			font.print(32.0f, 64.0f, 1.0f, "This is a test");
			glColor3ub(0xff, 0, 0);
			font.print(32.0f, 32.0f, 2.0f, "This is another test");
			m_video.update();
		}
	}
	return 0;
}

