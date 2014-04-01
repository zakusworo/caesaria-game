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
// Copyright 2012-2013 Dalerank, dalerankn8@gmail.com
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com

#ifndef __CAESARIA_PLAYERCITY_H_INCLUDED__
#define __CAESARIA_PLAYERCITY_H_INCLUDED__

#include "core/serializer.hpp"
#include "core/signals.hpp"
#include "gfx/tile.hpp"
#include "core/position.hpp"
#include "game/player.hpp"
#include "objects/constants.hpp"
#include "world/city.hpp"
#include "walker/constants.hpp"

class CityBuildOptions;
class CityTradeOptions;
class CityWinTargets;

namespace city
{
  class Funds;
}

struct BorderInfo
{
  TilePos roadEntry;
  TilePos roadExit;
  TilePos boatEntry;
  TilePos boatExit;
};

class PlayerCity : public world::City
{
public:
  static PlayerCityPtr create( world::EmpirePtr empire, PlayerPtr player );
  virtual ~PlayerCity();

  virtual void timeStep( unsigned int time );  // performs one simulation step

  void setLocation( const Point& location );
  Point getLocation() const;

  WalkerList getWalkers( constants::walker::Type type );
  WalkerList getWalkers( constants::walker::Type type, TilePos startPos, TilePos stopPos=TilePos( -1, -1 ) );

  void addWalker( WalkerPtr walker );

  void addService( city::SrvcPtr service );
  city::SrvcPtr findService( const std::string& name ) const;

  TileOverlayList& getOverlays();

  void setBorderInfo( const BorderInfo& info );
  const BorderInfo& borderInfo() const;

  PlayerPtr getPlayer() const;
  
  void setCameraPos(const TilePos pos);
  TilePos getCameraPos() const;
     
  ClimateType getClimate() const;
  void setClimate(const ClimateType);

  city::Funds& funds() const;

  int getPopulation() const;
  int getProsperity() const;
  int getCulture() const;
  int getPeace() const;

  int getFavour() const;
  void updateFavour( int value );

  Tilemap& tilemap();

  std::string getName() const; 
  void setName( const std::string& name );

  virtual void save( VariantMap& stream) const;
  virtual void load( const VariantMap& stream);

  // add construction
  void addOverlay(TileOverlayPtr overlay);
  TileOverlayPtr getOverlay( const TilePos& pos ) const;

  const CityBuildOptions& getBuildOptions() const;
  void setBuildOptions( const CityBuildOptions& options );

  const CityWinTargets& getWinTargets() const;
  void setWinTargets( const CityWinTargets& targets );

  CityTradeOptions& getTradeOptions();

  virtual void arrivedMerchant( world::MerchantPtr merchant );

  virtual const GoodStore& getSells() const;
  virtual const GoodStore& getBuys() const;
  virtual unsigned int getTradeType() const;
  virtual world::EmpirePtr empire() const;

  void updateRoads();
   
oc3_signals public:
  Signal1<int>& onPopulationChanged();
  Signal1<int>& onFundsChanged();
  Signal1<std::string>& onWarningMessage();
  Signal2<TilePos,std::string>& onDisasterEvent();
  Signal0<>& onChangeBuildingOptions();  

private:
  PlayerCity();

  class Impl;
  ScopedPtr< Impl > _d;
};

#endif //__CAESARIA_PLAYERCITY_H_INCLUDED__
