// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <set>
#include <cstdint>

#include <boost/optional.hpp>

#include <math/vector_3d.h>

#include <noggit/Selection.h>
#include <noggit/WMO.h>

#include <opengl/shader.fwd.hpp>

class Frustum;
class WMO;
class World;

struct ENTRY_MODF;

namespace noggit
{
  namespace mpq
  {
    class file;
  }
}

class WMOInstance
{
public:
  noggit::scoped_wmo_reference wmo;
  ::math::vector_3d pos;
  ::math::vector_3d  extents[2];
  ::math::vector_3d  dir;
  int mUniqueID;
  uint16_t mFlags;
  uint16_t mUnknown;
  uint16_t mNameset;
  uint16_t doodadset;

public:
  WMOInstance( World*, std::string const& path, noggit::mpq::file* _file );
  WMOInstance( World*, std::string const& path, ENTRY_MODF* d );
  explicit WMOInstance( World*, std::string const& path );
  ~WMOInstance();

  void draw ( opengl::scoped::use_program& shader
            , bool draw_fog
            , bool hasSkies
            , const float culldistance
            , const float& fog_distance
            , const Frustum& frustum
            , const ::math::vector_3d& camera
            , const boost::optional<selection_type>& selected_item
            ) const;
  void draw_doodads ( bool draw_fog
                    , const float culldistance
                    , const float& fog_distance
                    , const Frustum& frustum
                    , const ::math::vector_3d& camera
                    );

  void intersect (math::ray ray, selection_result& results);

  void resetDirection();

  void recalc_extents();

private:
  World* _world;
};
