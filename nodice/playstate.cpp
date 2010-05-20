/**
 * @file nodice/playstate.cpp
 * @brief Implemntation of the nodice/playstate module.
 *
 * Copyright 2010 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#include "nodice/playstate.h"

#include <iostream>
#include "nodice/colour.h"
#include "nodice/object.h"
#include "nodice/shape.h"
#include "nodice/video.h"


namespace
{
	static const GLfloat near =  0.0f;
	static const GLfloat far  = 10.0f;
	static const NoDice::Vector3f board_scale(1.0f/12.0f, 1.0f/12.0f, 1.0f/12.0f);
	static const NoDice::Vector3f board_pos(-4.0f, -6.0f, -2.0f);
} // anonymous namespace


NoDice::PlayState::
PlayState(Config& config)
: GameState(config)
, m_state(state_idle)
, m_gameboard(config)
, m_mouseIsDown(false)
{
}


NoDice::PlayState::
~PlayState()
{
}


void NoDice::PlayState::
pointerMove(int x, int y, int dx, int dy)
{
	if (m_mouseIsDown && m_state != state_swapping)
	{
		int x2 = m_selectedX;
		int y2 = m_selectedY;
		if (std::abs(dx) > std::abs(dy))
		{
			if (dx < 0 && m_selectedX >= 0)
					--x2;
			else if (m_selectedX < m_config.boardSize())
					++x2;
			else
				return;
		}
		else
		{
			if (dy < 0 && m_selectedY >= 0)
					--y2;
			else if (m_selectedY < m_config.boardSize())
					++y2;
			else
				return;
		}
		m_state = state_swapping;
		m_gameboard.startSwap(m_selectedX, m_selectedY, x2, y2);
	}
}


void NoDice::PlayState::
pointerClick(int x, int y, PointerAction action)
{
	if (action == pointerUp)
	{
		m_mouseIsDown = false;
	}
	else if (action == pointerDown)
	{
		float win_width = float(m_config.screenWidth()) / 2.0f;
		float win_height = float(m_config.screenHeight()) / 2.0f;
		float unit_x = float(x - win_width) / win_width;
		float unit_y = -float(y - win_height) / win_height;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glScalef(board_scale.x, board_scale.y, board_scale.z);
		glTranslatef(board_pos.x, board_pos.y, board_pos.z);
			Matrix4f projection;
			glGetFloatv(GL_PROJECTION_MATRIX, projection.array);
			Matrix4f modelview;
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview.array);
			Matrix4f unprojection;
			(projection * modelview).getInverse(unprojection);
		glPopMatrix();

		Vector4f ray(unit_x, unit_y, 0.0f, 1.0f);
		Vector4f beam = unprojection * ray;
		m_selectedX = int(beam.x / 2.0f + 0.50f);
		m_selectedY = int(beam.y / 2.0f + 0.50f);
		if (m_selectedX >= m_config.boardSize() || m_selectedX < 0)
			return;
		if (m_selectedY >= m_config.boardSize() || m_selectedY < 0)
			return;

		ObjectPtr obj = m_gameboard.at(m_selectedX, m_selectedY);
		obj->setHighlight(true);

		m_mouseIsDown = true;
	}
}


void NoDice::PlayState::
update(App& app)
{
	m_gameboard.update();
	if (m_state == state_swapping)
	{
		if (!m_gameboard.isSwapping())
		{
			if (m_gameboard.findWins())
			{
			}
			m_state = state_idle;
		}
	}
}


GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPosition[] = { 2.0f, 2.0f, 3.0f, 0.0f };
GLfloat lightDirection[] = { -2.0f, -2.0f, -3.0f };

void NoDice::PlayState::
draw(Video& video) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Adjust projection to take aspect ratio into account.
	int w = m_config.screenWidth();
	int h = m_config.screenHeight();
	float right = 1.0f;
	float top   = 1.0f;
	if (h < w)
		top = float(h) / float(w);
	else
		right = float(w) / float(h);
#ifdef HAVE_OPENGL_ES
	glFrustumf(-right, right, -top, top, near, far);
#else
	glOrtho(-right, right, -top, top, near, far);
#endif
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white.rgba);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0f);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white.rgba);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 1.2f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(board_scale.x, board_scale.y, board_scale.z);
	glTranslatef(board_pos.x, board_pos.y, board_pos.z);
	m_gameboard.draw();

	glPopMatrix();
}


