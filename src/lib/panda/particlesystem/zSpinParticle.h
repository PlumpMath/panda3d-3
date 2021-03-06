// Filename: zSpinParticle.h
// Created by:  charles (16Aug00)
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

#ifndef ZSPINPARTICLE_H
#define ZSPINPARTICLE_H

#include "baseParticle.h"

////////////////////////////////////////////////////////////////////
//       Class : ZSpinParticle
// Description : describes a particle that spins along its z axis.
//               this is kind of an intermediary class- if you're
//               using a SpriteParticleRenderer and you want your
//               sprites to spin without having them be full-blown
//               oriented (i.e. angry quat math), use this.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSICS ZSpinParticle : public BaseParticle {
public:
  ZSpinParticle();
  ZSpinParticle(const ZSpinParticle &copy);
  virtual ~ZSpinParticle();

  virtual PhysicsObject *make_copy() const;

  virtual void init();
  virtual void update();
  virtual void die();

  virtual float get_theta() const;

  INLINE void set_initial_angle(float t);
  INLINE float get_initial_angle() const;

  INLINE void set_final_angle(float t);
  INLINE float get_final_angle() const;

  // 'set_final_angle' and 'angular_velocity' are mutually exclusive apis
  // if angular-velocity is specified, final_angle is ignored
  INLINE void  set_angular_velocity(float v);
  INLINE float get_angular_velocity() const;

  INLINE void enable_angular_velocity(bool bEnabled);
  INLINE bool get_angular_velocity_enabled() const;

  virtual void output(ostream &out) const;
  virtual void write(ostream &out, int indent=0) const;

private:
  float _initial_angle;
  float _final_angle;
  float _cur_angle;
  float _angular_velocity;
  bool  _bUseAngularVelocity;
};

#include "zSpinParticle.I"

#endif // ZSPINPARTICLE_H
