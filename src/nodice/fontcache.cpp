/**
 * @file nodice/fontcache.cpp
 * @brief Implemntation of the nodice/fontcache module.
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
#include "nodice/fontcache.h"

#include "nodice/font.h"


NoDice::FontCache::FontCache()
{
}


NoDice::Font& NoDice::FontCache::get(const std::string& fontName, unsigned int pointSize)
{
  static Font testFont(DATA_DIR"assets/FreeSans.ttf", 18);
  return testFont;
}

