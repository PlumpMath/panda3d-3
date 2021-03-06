// Filename: tangentRingEmitter.h
// Created by:  charles (25Jul00)
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

#ifndef TANGENTRINGEMITTER_H
#define TANGENTRINGEMITTER_H

#include "baseParticleEmitter.h"

////////////////////////////////////////////////////////////////////
//       Class : TangentRingEmitter
// Description : Describes a planar ring region in which
//               tangent particles are generated, and particles
//               fly off tangential to the ring.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSICS TangentRingEmitter : public BaseParticleEmitter {
PUBLISHED:
  TangentRingEmitter();
  TangentRingEmitter(const TangentRingEmitter &copy);
  virtual ~TangentRingEmitter();

  virtual BaseParticleEmitter *make_copy();

  INLINE void set_radius(float r);
  INLINE void set_radius_spread(float spread);

  INLINE float get_radius() const;
  INLINE float get_radius_spread() const;

  virtual void output(ostream &out) const;
  virtual void write(ostream &out, int indent=0) const;

private:
  float _radius;
  float _radius_spread;

  // CUSTOM EMISSION PARAMETERS
  // none

  ///////////////////////////////
  // scratch variables that carry over from position calc to velocity calc
  float _x;
  float _y;
  ///////////////////////////////

  virtual void assign_initial_position(LPoint3f& pos);
  virtual void assign_initial_velocity(LVector3f& vel);
};

#include "tangentRingEmitter.I"

#endif // TANGENTRINGEMITTER_H
