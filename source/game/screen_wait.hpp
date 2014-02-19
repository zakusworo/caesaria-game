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
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com
// Copyright 2012-2013 Dalerank, dalerank@gmail.com

#ifndef _CAESARIA_SCREEN_WAIT_HPP_INCLUDE_
#define _CAESARIA_SCREEN_WAIT_HPP_INCLUDE_

#include "screen.hpp"
#include "core/scopedptr.hpp"

class GfxEngine;
class GuiEnv;

// displays a background image
class ScreenWait: public Screen
{
public:
  ScreenWait();
  ~ScreenWait();

  void initialize();

  virtual void draw();
  void fadeOut();

protected:
	int getResult() const;

private:
  class Impl;
  ScopedPtr<Impl> _d;
};


#endif //_CAESARIA_SCREEN_WAIT_HPP_INCLUDE_
