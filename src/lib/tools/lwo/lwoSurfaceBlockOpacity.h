// Filename: lwoSurfaceBlockOpacity.h
// Created by:  drose (24Apr01)
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

#ifndef LWOSURFACEBLOCKOPACITY_H
#define LWOSURFACEBLOCKOPACITY_H

#include "pandatoolbase.h"

#include "lwoChunk.h"

////////////////////////////////////////////////////////////////////
//       Class : LwoSurfaceBlockOpacity
// Description : Indicates how transparent or opaque this particular
//               layer is in relation to the layers beneath it.  This
//               is a subchunk of LwoSurfaceBlockHeader.
////////////////////////////////////////////////////////////////////
class LwoSurfaceBlockOpacity : public LwoChunk {
public:
  enum Type {
    T_additive             = 0,
    T_subtractive          = 1,
    T_difference           = 2,
    T_multiply             = 3,
    T_divide               = 4,
    T_alpha                = 5,
    T_texture_displacement = 6
  };

  Type _type;
  float _opacity;
  int _envelope;

public:
  virtual bool read_iff(IffInputFile *in, size_t stop_at);
  virtual void write(ostream &out, int indent_level = 0) const;

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    LwoChunk::init_type();
    register_type(_type_handle, "LwoSurfaceBlockOpacity",
                  LwoChunk::get_class_type());
  }

private:
  static TypeHandle _type_handle;
};

#endif


