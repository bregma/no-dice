/**
 * @file main.cpp
 * @brief Implemntation of the no-dice mainline.
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
#include <fstream>
#include <iostream>
#include "nodice/app.h"
#include <SDL/SDL_main.h>


namespace
{
	/**
	 * A class that futzes around wit cerr to force-redirect it to a log file.
	 * This is mostly for use on windows or under a GUI where stderr is not
	 * normally seen except by gooroos.
	 */
	class LogFutzer
	{
	public:
		// Redirects stderr into a log file.
		LogFutzer(std::ostream& stream)
		: m_stream(stream)
		, m_errbuf(m_stream.rdbuf())
		, m_logfile("no-dice.log")
		{
			m_stream.rdbuf(m_logfile.rdbuf());
		}

		// Restores stderr.
		~LogFutzer()
		{
			m_stream.rdbuf(m_errbuf);
		}

	private:
		std::ostream&   m_stream;
		std::streambuf* m_errbuf;
		std::ofstream   m_logfile;
	};
} // anonymous namespace


int main(int argc, char* argv[])
{
#if defined(_WIN32)
	LogFutzer out(std::cout);
	LogFutzer err(std::cerr);
#endif
	NoDice::App app(argc, argv);
	return app.run();
}

