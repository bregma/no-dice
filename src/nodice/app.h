/**
 * @file nodice/app.h
 * @brief Public interface of the nodice/app module.
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
#ifndef NODICE_APP_H
#define NODICE_APP_H 1

#include "nodice/video.h"


namespace NoDice
{
  class Config;

  /**
   * A wrapper class to provide application-wide setup and teardown.
   */
  class App
  {
  public:
    App(const Config& config);

    ~App();

    int run();

  private:
    /** @brief A special RAII object for SDL initialization and teardown. */
    struct SdlInit
    {
      SdlInit();
      ~SdlInit();
    };

    /** Performs gamewise updates. */
    void action();

    /** Performs screen updates. */
    void draw();

    void pointerDown(int x, int y);
    void pointerMove(int x, int y, int dx, int dy);
    void pointerUp(int x, int y);

  private:
    SdlInit  m_sdlInit;
    Video    m_video;
    bool     m_isMovingObject;
    float    m_tx;
		float    m_ty;
  };

} // namespace NoDice
#endif // NODICE_APP_H

