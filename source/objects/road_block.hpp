// This file is part of CaesarIA.
//
// CaesarIA is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CaesarIA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CaesarIA.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#ifndef __CAESARIA_ROADBLOCK_H_INCLUDED__
#define __CAESARIA_ROADBLOCK_H_INCLUDED__

#include "objects/road.hpp"

class RoadBlock : public Road
{
public:
  RoadBlock();
  virtual bool canBuild(const CityAreaInfo& areaInfo) const;
  virtual const gfx::Picture& picture(const CityAreaInfo& areaInfo) const;
  virtual void appendPaved(int value);
  virtual bool build(const CityAreaInfo &info);
  virtual void save(VariantMap &stream) const;
  virtual void load(const VariantMap &stream);
  virtual const gfx::Picture& picture() const;
};

#endif //__CAESARIA_ROADBLOCK_H_INCLUDED__