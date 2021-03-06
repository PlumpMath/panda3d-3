// Filename: collisionParabola.h
// Created by:  drose (11Oct07)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef COLLISIONPARABOLA_H
#define COLLISIONPARABOLA_H

#include "pandabase.h"

#include "collisionSolid.h"
#include "parabola.h"

class LensNode;

////////////////////////////////////////////////////////////////////
//       Class : CollisionParabola
// Description : This defines a parabolic arc, or subset of an arc,
//               similar to the path of a projectile or falling
//               object.  It is finite, having a specific beginning
//               and end, but it is infinitely thin.
//
//               Think of it as a wire bending from point t1 to point
//               t2 along the path of a pre-defined parabola.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_COLLIDE CollisionParabola : public CollisionSolid {
PUBLISHED:
  INLINE CollisionParabola();
  INLINE CollisionParabola(const Parabolaf &parabola, float t1, float t2);

  virtual LPoint3f get_collision_origin() const;

public:
  INLINE CollisionParabola(const CollisionParabola &copy);
  virtual CollisionSolid *make_copy();

  virtual PT(CollisionEntry)
  test_intersection(const CollisionEntry &entry) const;

  virtual void xform(const LMatrix4f &mat);

  virtual PStatCollector &get_volume_pcollector();
  virtual PStatCollector &get_test_pcollector();

  virtual void output(ostream &out) const;

PUBLISHED:
  INLINE void set_parabola(const Parabolaf &parabola);
  INLINE const Parabolaf &get_parabola() const;

  INLINE void set_t1(float t1);
  INLINE float get_t1() const;

  INLINE void set_t2(float t2);
  INLINE float get_t2() const;

protected:
  virtual PT(BoundingVolume) compute_internal_bounds() const;

protected:
  virtual void fill_viz_geom();

private:
  Parabolaf _parabola;
  float _t1, _t2;

  static PStatCollector _volume_pcollector;
  static PStatCollector _test_pcollector;

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &dg);

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    CollisionSolid::init_type();
    register_type(_type_handle, "CollisionParabola",
                  CollisionSolid::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "collisionParabola.I"

#endif


