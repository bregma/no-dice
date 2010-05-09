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

#include "nodice/colour.h"
#include "nodice/object.h"
#include "nodice/shape.h"
#include "nodice/video.h"


NoDice::PlayState::
PlayState(Config& config)
: GameState(config)
{
}


NoDice::PlayState::
~PlayState()
{
}

int xrot = 0.0f, yrot = 0.0f;

void NoDice::PlayState::
update(App& app)
{
	xrot = (xrot + 1) % 360;
	yrot = (yrot + 6) % 360;
}


GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightPosition[] = { 2.0f, 2.0f, 3.0f, 0.0f };
GLfloat lightDirection[] = { -2.0f, -2.0f, -3.0f };

void NoDice::PlayState::
draw(Video& video)
{
	// temporary, for testing
	static NoDice::Object s_object(NoDice::chooseAShape());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef HAVE_OPENGL_ES
	glFrustumf(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
#else
	glFrustum(0.0f, 1.0f, 0.0f, 1.0f, -2.0f, 1.0f);
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
	glLoadIdentity();
	glScalef(1.0f/4.0f, 1.0f/4.0f, 1.0f/4.0f);
	glRotatef(float(xrot), 1.0f, 0.0f, 0.0f);
	glRotatef(float(yrot), 0.0f, 1.0f, 0.0f);

	s_object.draw();
}


