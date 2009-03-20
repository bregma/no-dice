/**
 * @file nodice/videocontext/g.h
 * @brief Public interface of the nodice/videocontext/g module.
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
#ifndef NODICE_VIDEOCONTEXT_H
#define NODICE_VIDEOCONTEXT_H 1

namespace NoDice
{
	/**
	 * A base class for the various platform-specific video contexts.
	 */
	class VideoContext
	{
	public:
		virtual ~VideoContext() { }

		virtual void swapBuffers() = 0;

		int width()  const { return m_width; }
		int height() const { return m_height; }
		int depth()  const { return m_depth; }

	protected:
		int m_width;
		int m_height;
		int m_depth;
	};
} // namespace NoDice

#endif // NODICE_VIDEOCONTEXT_H
