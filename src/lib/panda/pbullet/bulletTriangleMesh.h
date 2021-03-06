// Filename: bulletTriangleMesh.h
// Created by:  enn0x (09Feb10)
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

#ifndef __BULLET_TRIANGLE_MESH_H__
#define __BULLET_TRIANGLE_MESH_H__

#include "pandabase.h"

#include "bullet_includes.h"
#include "bullet_utils.h"

#include "typedReferenceCount.h"
#include "nodePath.h"
#include "luse.h"
#include "geom.h"
#include "pta_LVecBase3f.h"
#include "pta_int.h"

////////////////////////////////////////////////////////////////////
//       Class : BulletTriangleMesh
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDABULLET BulletTriangleMesh : public TypedReferenceCount {

PUBLISHED:
  BulletTriangleMesh();
  INLINE ~BulletTriangleMesh();

  void add_triangle(const LPoint3f &p0,
                    const LPoint3f &p1,
                    const LPoint3f &p2,
                    bool remove_duplicate_vertices=false);
  void add_array(const PTA_LVecBase3f &points,
                 const PTA_int &indices,
                 bool remove_duplicate_vertices=false);
  void add_geom(const Geom *geom, 
                bool remove_duplicate_vertices=false);

  void set_welding_distance(PN_stdfloat distance);
  void preallocate(int num_verts, int num_indices);

  int get_num_triangles() const;
  PN_stdfloat get_welding_distance() const;

  virtual void output(ostream &out) const;
  virtual void write(ostream &out, int indent_level) const;

public:
  INLINE btTriangleMesh *ptr() const;

private:
  btTriangleMesh *_mesh;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "BulletTriangleMesh", 
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

INLINE ostream &operator << (ostream &out, const BulletTriangleMesh &obj);

#include "bulletTriangleMesh.I"

#endif // __BULLET_TRIANGLE_MESH_H__
