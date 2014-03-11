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
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#include "tilemap.hpp"

#include "gfx/tile.hpp"
#include "objects/building.hpp"
#include "core/exception.hpp"
#include "core/position.hpp"
#include "core/variant.hpp"
#include "core/stringhelper.hpp"
#include "core/foreach.hpp"
#include "core/logger.hpp"

static Tile invalidTile = Tile( TilePos( -1, -1 ) );
typedef std::vector< Tile > Row;

class TileGrid : public std::vector< Row >
{
};

class Tilemap::Impl : public TileGrid
{
public:
  Tile& at( const int i, const int j )
  {
    if( isInside( TilePos( i, j ) ) )
    {
      return (*this)[i][j];
    }

    //Logger::warning( "Need inside point current=[%d, %d]", i, j );
    return invalidTile;
  }

  bool isInside( const TilePos& pos )
  {
    return( pos.i() >= 0 && pos.j()>=0 && pos.i() < size && pos.j() < size);
  }

  void resize( const int s )
  {
    size = s;

    // resize the tile array
    TileGrid::resize( size );
    for( int i = 0; i < size; ++i )
    {
      (*this)[i].reserve( size );

      for (int j = 0; j < size; ++j)
      {
        (*this)[i].push_back( Tile( TilePos( i, j ) ));
      }
    }
  }

  int size;
};

Tilemap::Tilemap() : _d( new Impl )
{
  _d->size = 0;
}

void Tilemap::resize( const int size )
{
  _d->resize( size );
}

bool Tilemap::isInside(const TilePos& pos ) const
{
  return _d->isInside( pos );
}

TilePos Tilemap::fit( const TilePos& pos ) const
{
  TilePos ret;
  ret.setI( math::clamp( pos.i(), 0, _d->size ) );
  ret.setJ( math::clamp( pos.j(), 0, _d->size ) );
  return ret;
}

Tile* Tilemap::at(Point pos, bool overborder)
{
  // x relative to the left most pixel of the tilemap
  int i = (pos.x() + 2 * pos.y()) / 60;
  int j = (pos.x() - 2 * pos.y()) / 60;

  if( overborder )
  {
      i = math::clamp( i, 0, getSize() - 1 );
      j = math::clamp( j, 0, getSize() - 1 );
  }
  // std::cout << "ij ("<<i<<","<<j<<")"<<std::endl;

  if (i>=0 && j>=0 && i < getSize() && j < getSize())
  {
    // valid coordinate
    return &at( TilePos( i, j ) );
  }
  else // the pixel is outside the tilemap => no tile here
  {
     return NULL;
  }
}

Tile& Tilemap::at(const int i, const int j)
{
  return _d->at( i, j );
}

const Tile& Tilemap::at(const int i, const int j) const
{
  return const_cast< Tilemap* >( this )->at( i, j );
}

Tile& Tilemap::at( const TilePos& ij )
{
  return _d->at( ij.i(), ij.j() );
}

const Tile& Tilemap::at( const TilePos& ij ) const
{
  return const_cast<Tilemap*>( this )->at( ij.i(), ij.j() );
}

int Tilemap::getSize() const
{
  return _d->size;
}

TilesArray Tilemap::getRectangle( const TilePos& start, const TilePos& stop, const bool corners /*= true*/ )
{
  TilesArray res;

  int delta_corners = 0;
  if (! corners)
  {
    delta_corners = 1;
  }

  for(int i = start.i() + delta_corners; i <= stop.i() - delta_corners; ++i)
  {
    if (isInside( TilePos( i, start.j() ) ))
    {
      res.push_back( &at(i, start.j() ));
    }

    if (isInside( TilePos( i, stop.j() ) ))
    {
      res.push_back( &at( i, stop.j() ));
    }
  }

  for (int j = start.j() + 1; j <= stop.j() - 1; ++j)  // corners have been handled already
  {
    if (isInside( TilePos( start.i(), j ) ))
    {
      res.push_back(&at(start.i(), j));
    }

    if (isInside( TilePos( stop.i(), j ) ))
    {
      res.push_back(&at(stop.i(), j));
    }
  }

  return res;
}

TilesArray Tilemap::getRectangle( TilePos pos, Size size, const bool corners /*= true */ )
{
  return getRectangle( pos, pos + TilePos( size.width()-1, size.height()-1), corners );
}

// Get tiles inside of rectangle
TilesArray Tilemap::getArea(TilePos start, TilePos stop )
{
  TilesArray res;
  res.reserve( 100 );

  for (int i = start.i(); i <= stop.i(); ++i)
  {
    for (int j = start.j(); j <= stop.j(); ++j)
    {
      if( isInside( TilePos( i, j ) ))
      {
        res.push_back( &at( TilePos( i, j ) ) );
      }
    }
  }

  return res;
}

TilesArray Tilemap::getArea( TilePos start, Size size )
{
  return getArea( start, start + TilePos( size.width()-1, size.height()-1 ) );
}

void Tilemap::save( VariantMap& stream ) const
{
  // saves the graphics map
  VariantList bitsetInfo;
  VariantList desInfo;
  VariantList idInfo;

  TilesArray tiles = const_cast< Tilemap* >( this )->getArea( TilePos( 0, 0 ), Size( _d->size ) );
  foreach( it, tiles )
  {
    Tile* tile = *it;
    bitsetInfo.push_back( TileHelper::encode( *tile ) );
    desInfo.push_back( tile->getDesirability() );
    idInfo.push_back( tile->getOriginalImgId() );
  }

  stream[ "bitset" ]       = bitsetInfo;
  stream[ "desirability" ] = desInfo;
  stream[ "imgId" ]        = idInfo;
  stream[ "size" ]         = _d->size;
}

void Tilemap::load( const VariantMap& stream )
{
  VariantList bitsetInfo = stream.get( "bitset" ).toList();
  VariantList desInfo    = stream.get( "desirability" ).toList();
  VariantList idInfo     = stream.get( "imgId" ).toList();

  int size = stream.get( "size" ).toInt();

  resize( size );

  VariantList::iterator imgIdIt        = idInfo.begin();
  VariantList::iterator bitsetInfoIt   = bitsetInfo.begin();
  VariantList::iterator desirabilityIt = desInfo.begin();

  TilesArray tiles = getArea( TilePos( 0, 0 ), Size( _d->size ) );
  for( TilesArray::iterator it = tiles.begin(); it != tiles.end();
       it++, imgIdIt++, bitsetInfoIt++, desirabilityIt++ )
  {
    Tile* tile = *it;

    TileHelper::decode( *tile, (*bitsetInfoIt).toInt() );
    tile->appendDesirability( (*desirabilityIt).toInt() );

    int imgId = (*imgIdIt).toInt();
    if( !tile->getMasterTile() && imgId != 0 )
    {
      std::string picName = TileHelper::convId2PicName( imgId );
      Picture& pic = Picture::load( picName );

      tile->setOriginalImgId( imgId );

      int tile_size = (pic.width()+2)/60;  // size of the multi-tile. the multi-tile is a square.

      // master is the left-most subtile
      Tile* master = (tile_size == 1) ? NULL : tile;

      for ( int di = 0; di<tile_size; ++di )
      {
        // for each subrow of the multi-tile
        for (int dj = 0; dj<tile_size; ++dj)
        {
          // for each subcol of the multi-tile
          Tile &sub_tile = at( tile->pos() + TilePos( di, dj ) );
          sub_tile.setMasterTile( master );
          sub_tile.setPicture( &pic );
        }
      }
    }
  }
}

Tilemap::~Tilemap()
{

}
