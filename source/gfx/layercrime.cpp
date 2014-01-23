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

#include "layercrime.hpp"
#include "tileoverlay.hpp"
#include "objects/constants.hpp"
#include "objects/house.hpp"
#include "objects/house_level.hpp"
#include "game/resourcegroup.hpp"
#include "city/helper.hpp"
#include "layerconstants.hpp"

using namespace constants;

int LayerCrime::getType() const
{
  return citylayer::crime;
}

Layer::VisibleWalkers LayerCrime::getVisibleWalkers() const
{
  VisibleWalkers ret;
  ret.insert( walker::prefect );
  return ret;
}

void LayerCrime::drawTile(GfxEngine& engine, Tile& tile, Point offset)
{
  Point screenPos = tile.getXY() + offset;

  tile.setWasDrawn();

  bool needDrawAnimations = false;
  if( tile.getOverlay().isNull() )
  {
    //draw background
    engine.drawPicture( tile.getPicture(), screenPos );
  }
  else
  {
    TileOverlayPtr overlay = tile.getOverlay();
    int fireLevel = -1;
    switch( overlay->getType() )
    {
    //fire buildings and roads
    case construction::road:
    case construction::plaza:
    case building::prefecture:
      engine.drawPicture( tile.getPicture(), screenPos );
      needDrawAnimations = true;
    break;

      //houses
    case building::house:
      {
        HousePtr house = ptr_cast<House>( overlay );
        fireLevel = (int)house->getServiceValue( Service::crime );
        needDrawAnimations = (house->getSpec().getLevel() == 1) && house->getHabitants().empty();

        CityHelper helper( _getCity() );
        drawArea( engine, helper.getArea( overlay ), offset, ResourceGroup::foodOverlay, OverlayPic::inHouseBase  );
      }
    break;

      //other buildings
    default:
      {
        CityHelper helper( _getCity() );
        drawArea( engine, helper.getArea( overlay ), offset, ResourceGroup::foodOverlay, OverlayPic::base  );
      }
    break;
    }

    if( needDrawAnimations )
    {
      registerTileForRendering( tile );
    }
    else if( fireLevel >= 0)
    {
      drawColumn( engine, screenPos, fireLevel );
    }
  }
}

LayerPtr LayerCrime::create(TilemapCamera& camera, PlayerCityPtr city)
{
  LayerPtr ret( new LayerCrime( camera, city ) );
  ret->drop();

  return ret;
}

LayerCrime::LayerCrime(TilemapCamera& camera, PlayerCityPtr city)
  : Layer( camera, city )
{
  _loadColumnPicture( 18 );
}
