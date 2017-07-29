/**
 * @file nodice/videocontext.h
 * @brief Public interface of the nodice/videocontext/g module.
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
    VideoContext()
    : depth_(32)
    { }

    virtual
    ~VideoContext()
    { }


    virtual void
    swapBuffers() = 0;

    int
    depth() const
    { return depth_; }

  protected:
    int depth_;
  };
} // namespace NoDice

#endif // NODICE_VIDEOCONTEXT_H
