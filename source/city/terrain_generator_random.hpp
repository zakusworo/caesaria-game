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

#ifndef _CAESARIA_TERRAIN_GENERATOR_RANDOM_INCLUDE_H_
#define _CAESARIA_TERRAIN_GENERATOR_RANDOM_INCLUDE_H_

#include <time.h>
#include <cstdlib>
#include <cmath>

class Random {
 public:
  Random();
  Random(int seed);
  Random(double seed);
  Random(float seed);
  ~Random() {}
  int Integer(int minimum, int maximum);
  double Double(double minimum, double maximum);
  float Float(float minimum, float maximum);
};


#endif //_CAESARIA_TERRAIN_GENERATOR_RANDOM_INCLUDE_H_
