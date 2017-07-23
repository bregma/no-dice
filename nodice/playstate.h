/**
 * @file nodice/playstate.h
 * @brief Public interface of the nodice/playstate module.
 *
 * Copyright 2010, 2011 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_PLAYSTATE_H
#define NODICE_PLAYSTATE_H 1

#include "nodice/gamestate.h"

#include "nodice/board.h"
#include "nodice/maths.h"
#include <string>
#include <vector>


namespace NoDice
{
  class Font;

  class PlayState
  : public GameState
  {
  public:
    PlayState(Config& config);
    ~PlayState();

    void pointerMove(int x, int y, int dx, int dy);
    void pointerClick(int x, int y, PointerAction action);

    void update(App& app);
    void draw(Video& video);

  private:
    void calculateScore(const ObjectBrace& matches);

  private:
    enum SubState
    {
      state_idle,
      state_swapping,
      state_unswapping,
      state_replacing,
      state_rotating,
      state_end
    };

    SubState                  m_state;
    Board                     m_gameboard;
    Font&                     m_scoreFont;
    bool                      m_mouseIsDown;
    Vector2i                  m_mouseDownPos;
    Vector2i                  m_selectedPos;
    int                       m_multiplier;
    int                       m_score;
    std::vector<std::string>  m_winMessages;
    Matrix4f                  m_unproject;
  };

} // namespace noDice

#endif // NODICE_PLAYSTATE_H
