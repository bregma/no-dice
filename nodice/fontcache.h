/**
 * @file fontcache.h
 * @brief Public interface of the nodice/fontcache module.
 */
/*
 * Copyright 2017 Stephen M. Webb  <stephen.webb@bregmasoft.ca>
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
#ifndef NODICE_FONTCACHE_H
#define NODICE_FONTCACHE_H 1

#include <memory>
#include <string>
#include <vector>


namespace NoDice
{
class App;
class Font;

/**
 * A cache of font objects.  If a requested font is not present in the cache, it
 * gets loaded in from where fonts getr loaded in from.
 */
class FontCache
{
public:
  FontCache(App* app);

  ~FontCache();

  /**
   * Gets a font with a given typeface name and pointsize.
   * @param[in] the name of the typeface
   * @param[in] the pointsize of the font
   */
  Font&
  get_font(std::string const& typeface, unsigned pointsize);

private:
  struct Entry
  {
    std::string           name;
    std::unique_ptr<Font> font;
  };

  using Cache = std::vector<Entry>;

private:
  App*   app_;
  Cache  cache_;
};


} // namespace NoDice

#endif // NODICE_FONTCACHE_H
