// Filename: bulletTriangleMeshShape.h
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

#ifndef __BULLET_TRIANGLE_MESH_SHAPE_H__
#define __BULLET_TRIANGLE_MESH_SHAPE_H__

#include "pandabase.h"

#include "bullet_includes.h"
#include "bulletShape.h"

#include "luse.h"

class BulletTriangleMesh;

////////////////////////////////////////////////////////////////////
//       Class : BulletTriangleMeshShape
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDABULLET BulletTriangleMeshShape : public BulletShape {

PUBLISHED:
  BulletTriangleMeshShape(BulletTriangleMesh *mesh, bool dynamic, bool compress=true, bool bvh=true);
  INLINE BulletTriangleMeshShape(const BulletTriangleMeshShape &copy);
  INLINE void operator = (const BulletTriangleMeshShape &copy);
  INLINE ~BulletTriangleMeshShape();

  void refit_tree(const LPoint3f &aabb_min, const LPoint3f &aabb_max);

  INLINE bool is_static() const;
  INLINE bool is_dynamic() const;

public:
  virtual btCollisionShape *ptr() const;

private:
  btBvhTriangleMeshShape *_bvh_shape;
  btGImpactMeshShape *_gimpact_shape;

  PT(BulletTriangleMesh) _mesh;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    BulletShape::init_type();
    register_type(_type_handle, "BulletTriangleMeshShape", 
                  BulletShape::get_class_type());
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

#include "bulletTriangleMeshShape.I"

#endif // __BULLET_TRIANGLE_MESH_SHAPE_H__
