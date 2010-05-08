/**
 * @file nodice/config.h
 * @brief Public interface of the nodice/config module.
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
#ifndef NODICE_CONFIG_H
#define NODICE_CONFIG_H 1

namespace NoDice
{
  /**
   * Application-wide configuration.
   */
  class Config
  {
  public:
	  /** Construcrs a Config object from command-line arguments. */
	  Config(int argc, char* argv[]);

	  /** Destroys a Config object. */
	  ~Config();

		/** Indicates if debug mode is enabled. */
		bool isDebugMode() const;

		/** Indicates if fullscreen mode is active. */
		bool isFullscreen() const;

	private:
		bool  m_isDebugMode;
		bool  m_isFullscreen;
  };
} // namespace NoDice

#endif // NODICE_CONFIG_H
