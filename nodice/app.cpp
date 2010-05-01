/**
 * @file nodice/app.cpp
 * @brief Implemntation of the nodice/app module.
 *
 * Copyright 2009, 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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

#include <cassert>
#include <cstdlib>
#include <iostream>
#include "nodice/config.h"
#include "nodice/introstate.h"
#include <SDL/SDL.h>


namespace
{
	static const Uint32 UPDATE_TICKS         = 1000/44;
	static const Uint32 ACTIVE_FRAME_DELAY   = 10;
	static const Uint32 INACTIVE_FRAME_DELAY = 300;
} // anonymous namespace


NoDice::App::SdlInit::
SdlInit()
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


NoDice::App::SdlInit::
~SdlInit()
{
	::SDL_Quit();
}

/**
 * The App object takes care of initializing the even system.
 */
NoDice::App::
App(const NoDice::Config& config)
: m_sdlInit()
, m_video(config)
{
	m_stateStack.push(GameStatePtr(new IntroState));
}


NoDice::App::
~App()
{
}

int NoDice::App::
run()
{
	bool done = false;
	bool isActive = true;
	Uint32 epochTics = SDL_GetTicks();
	while (!done)
	{
		assert(!m_stateStack.empty());

		Uint32 currentTics = SDL_GetTicks();
		Uint32 deltaTics = currentTics - epochTics;
		if (deltaTics > UPDATE_TICKS)
		{
			action();
			epochTics = currentTics;
		}
		draw();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_ACTIVEEVENT:
				isActive = (event.active.gain != 0);
				if (isActive)
				{
					m_stateStack.top()->resume();
				}
				else
				{
					m_stateStack.top()->pause();
				}
				break;
			case SDL_MOUSEMOTION:
				m_stateStack.top()->pointerMove(event.motion.x, event.motion.y,
										                    event.motion.xrel, event.motion.yrel);
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_stateStack.top()->pointerClick(event.button.x, event.button.y,
																				GameState::pointerDown);
				break;
			case SDL_MOUSEBUTTONUP:
				m_stateStack.top()->pointerClick(event.button.x, event.button.y,
																				GameState::pointerUp);
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

		SDL_Delay(isActive ? ACTIVE_FRAME_DELAY : INACTIVE_FRAME_DELAY);
	}
	return 0;
}


void NoDice::App::
action()
{
	assert(!m_stateStack.empty());
	m_stateStack.top()->update();
}


void NoDice::App::
draw()
{
	assert(!m_stateStack.empty());

	m_stateStack.top()->draw(m_video);
	m_video.update();
}


