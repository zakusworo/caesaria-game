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

#ifndef __CAESARIA_MOVABLEOBJECT_H_INCLUDED__
#define __CAESARIA_MOVABLEOBJECT_H_INCLUDED__

#include "predefinitions.hpp"
#include "object.hpp"
#include "core/position_array.hpp"

namespace world
{

class Route : public PointsArray
{
public:
  unsigned int step;

  void reset();
};

class MovableObject : public Object
{
public:
  enum { defaultSpeed=1, defaultViewDistance=40 };
  virtual ~MovableObject();

  virtual void save( VariantMap& stream ) const;
  virtual void load( const VariantMap& stream );
  virtual void setSpeed( float speed );
  virtual void timeStep(const unsigned int time);
  virtual int searchRange() const;
  virtual const Route& way() const;

protected:
  virtual bool _findWay( Point p1, Point p2 );
  virtual void _reachedWay();
  virtual void _noWay();

  Route& _way();
  MovableObject( EmpirePtr empire );

private: 
  __DECLARE_IMPL(MovableObject)
};

class Messenger : public MovableObject
{
public:
  virtual ~Messenger();
  static void now( EmpirePtr empire,
                   const std::string& cityname,
                   const std::string& title,
                   const std::string& message );

  std::string title() const;
  std::string message() const;

private:
  Messenger( EmpirePtr empire );
  __DECLARE_IMPL(Messenger)
};

}
#endif //__CAESARIA_ARMY_H_INCLUDED__
