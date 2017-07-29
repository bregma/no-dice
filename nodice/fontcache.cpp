#include <iostream>
/**
 * @file fontcache.cpp
 * @brief Implementation of the nodice/fontcache module.
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
#include "nodice_config.h"
#include "nodice/fontcache.h"

#include <algorithm>
#include "nodice/config.h"
#include "nodice/font.h"
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>


namespace
{
  std::string
  font_key_for_typeface_and_size(std::string const& typeface, unsigned pointsize)
  {
    std::ostringstream ostr;
    ostr << typeface << '_' << pointsize;
    return ostr.str();
  }

  std::string
  ttf_filename_for_typeface(std::string const& typeface)
  {
    return typeface + ".ttf";
  }

} // anonymous namespace


NoDice::FontCache::
FontCache(Config const* config)
: config_(config)
{ }


NoDice::FontCache::
~FontCache()
{ }


NoDice::Font& NoDice::FontCache::
get_font(std::string const& typeface, unsigned pointsize)
{
  auto const& font_key = font_key_for_typeface_and_size(typeface, pointsize);
  auto const& it = std::find_if(std::begin(cache_), std::end(cache_),
                                [&font_key](Cache::value_type const& entry)
                                {
                                  return font_key == entry.name;
                                });
  if (it != std::end(cache_))
  {
    return *it->font;
  }

  auto const& ttf_filename = ttf_filename_for_typeface(typeface);
  for (auto const& path: config_->asset_search_path())
  {
    std::string filename = path + "/" + ttf_filename;
    if (0 == ::access(filename.c_str(), R_OK))
    {
      cache_.push_back(Cache::value_type{font_key, std::make_unique<Font>(filename, pointsize)});
      return *cache_.back().font;
    }
  }

  throw std::runtime_error("unable to find font '" + typeface + "'");
}


