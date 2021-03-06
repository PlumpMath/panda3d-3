// Filename: renderModeAttrib.h
// Created by:  drose (14Mar02)
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

#ifndef RENDERMODEATTRIB_H
#define RENDERMODEATTRIB_H

#include "pandabase.h"

#include "renderAttrib.h"
#include "geom.h"

class FactoryParams;

////////////////////////////////////////////////////////////////////
//       Class : RenderModeAttrib
// Description : Specifies how polygons are to be drawn.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH RenderModeAttrib : public RenderAttrib {
PUBLISHED:
  enum Mode {
    M_unchanged,

    // Normal, filled polygons.
    M_filled,

    // Wireframe polygons, possibly with thickness.
    M_wireframe,

    // Points at vertices only, possibly with thickness and/or
    // perspective sizing.
    M_point,

    // Filled polygons, without any particular emphasis on perspective
    // correctness (a particularly useful designation for software
    // rendering sprites).
    M_filled_flat
  };

private:
  INLINE RenderModeAttrib(Mode mode, float thickness, bool perspective);

PUBLISHED:
  static CPT(RenderAttrib) make(Mode mode, float thickness = 1.0f,
                                bool perspective = false);
  static CPT(RenderAttrib) make_default();

  INLINE Mode get_mode() const;
  INLINE float get_thickness() const;
  INLINE bool get_perspective() const;

  INLINE int get_geom_rendering(int geom_rendering) const;

public:
  virtual void output(ostream &out) const;

protected:
  virtual int compare_to_impl(const RenderAttrib *other) const;
  virtual CPT(RenderAttrib) compose_impl(const RenderAttrib *other) const;

private:
  Mode _mode;
  float _thickness;
  bool _perspective;

PUBLISHED:
  static int get_class_slot() {
    return _attrib_slot;
  }
  virtual int get_slot() const {
    return get_class_slot();
  }

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
    RenderAttrib::init_type();
    register_type(_type_handle, "RenderModeAttrib",
                  RenderAttrib::get_class_type());
    _attrib_slot = register_slot(_type_handle, 100, make_default);
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
  static int _attrib_slot;
};

#include "renderModeAttrib.I"

#endif

