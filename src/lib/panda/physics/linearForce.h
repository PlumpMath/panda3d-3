// Filename: linearForce.h
// Created by:  charles (13Jun00)
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

#ifndef LINEARFORCE_H
#define LINEARFORCE_H

#include "baseForce.h"

////////////////////////////////////////////////////////////////////
//       Class : LinearForce
// Description : A force that acts on a PhysicsObject by way of an
//               Integrator.  This is a pure virtual base class.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSICS LinearForce : public BaseForce {
PUBLISHED:
  ~LinearForce();

  INLINE void set_amplitude(const float a);
  INLINE void set_mass_dependent(bool m);

  INLINE float get_amplitude() const;
  INLINE bool get_mass_dependent() const;

  INLINE void set_vector_masks(bool x, bool y, bool z);
  INLINE LVector3f get_vector_masks();

  LVector3f get_vector(const PhysicsObject *po);

  virtual LinearForce *make_copy() = 0;

  virtual bool is_linear() const;
  
  virtual void output(ostream &out) const;
  virtual void write(ostream &out, unsigned int indent=0) const;

protected:
  LinearForce(float a, bool mass);
  LinearForce(const LinearForce& copy);

private:
  float _amplitude;
  bool _mass_dependent;

  bool _x_mask;
  bool _y_mask;
  bool _z_mask;

  virtual LVector3f get_child_vector(const PhysicsObject *po) = 0;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    BaseForce::init_type();
    register_type(_type_handle, "LinearForce",
                  BaseForce::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "linearForce.I"

#endif // BASELINEARFORCE_H
