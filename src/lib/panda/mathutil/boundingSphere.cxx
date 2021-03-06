// Filename: boundingSphere.cxx
// Created by:  drose (01Oct99)
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

#include "boundingSphere.h"
#include "boundingBox.h"
#include "boundingHexahedron.h"
#include "boundingLine.h"
#include "boundingPlane.h"
#include "config_mathutil.h"
#include "dcast.h"

#include <math.h>
#include <algorithm>

TypeHandle BoundingSphere::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::make_copy
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
BoundingVolume *BoundingSphere::
make_copy() const {
  return new BoundingSphere(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::get_min
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
LPoint3f BoundingSphere::
get_min() const {
  nassertr(!is_empty(), LPoint3f(0.0f, 0.0f, 0.0f));
  nassertr(!is_infinite(), LPoint3f(0.0f, 0.0f, 0.0f));
  return LPoint3f(_center[0] - _radius,
                  _center[1] - _radius,
                  _center[2] - _radius);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::get_max
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
LPoint3f BoundingSphere::
get_max() const {
  nassertr(!is_empty(), LPoint3f(0.0f, 0.0f, 0.0f));
  nassertr(!is_infinite(), LPoint3f(0.0f, 0.0f, 0.0f));
  return LPoint3f(_center[0] + _radius,
                  _center[1] + _radius,
                  _center[2] + _radius);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::get_volume
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
float BoundingSphere::
get_volume() const {
  nassertr(!is_infinite(), 0.0f);
  if (is_empty()) {
    return 0.0f;
  }

  // Volume of a sphere: four-thirds pi r cubed.
  return 4.0f / 3.0f * MathNumbers::pi_f * _radius * _radius * _radius;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::get_approx_center
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
LPoint3f BoundingSphere::
get_approx_center() const {
  nassertr(!is_empty(), LPoint3f(0.0f, 0.0f, 0.0f));
  nassertr(!is_infinite(), LPoint3f(0.0f, 0.0f, 0.0f));
  return get_center();
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::xform
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void BoundingSphere::
xform(const LMatrix4f &mat) {
  nassertv(!mat.is_nan());

  if (!is_empty() && !is_infinite()) {
    // First, determine the longest axis of the matrix, in case it
    // contains a non-uniform scale.

/*
    LVector3f x,y,z;
        mat.get_row3(x,0);
        mat.get_row3(y,1);
        mat.get_row3(z,2);

    float xd = dot(x, x);
    float yd = dot(y, y);
    float zd = dot(z, z);
*/
    float xd,yd,zd,scale;

#define ROW_DOTTED(mat,ROWNUM)                        \
            (mat._m.m._##ROWNUM##0*mat._m.m._##ROWNUM##0 +    \
             mat._m.m._##ROWNUM##1*mat._m.m._##ROWNUM##1 +    \
             mat._m.m._##ROWNUM##2*mat._m.m._##ROWNUM##2)
    
    xd = ROW_DOTTED(mat,0);
    yd = ROW_DOTTED(mat,1);
    zd = ROW_DOTTED(mat,2);

    scale = max(xd,yd);
    scale = max(scale,zd);
    scale = sqrtf(scale);

    // Transform the radius
    _radius *= scale;

    // Transform the center
    _center = _center * mat;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::output
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void BoundingSphere::
output(ostream &out) const {
  if (is_empty()) {
    out << "bsphere, empty";
  } else if (is_infinite()) {
    out << "bsphere, infinite";
  } else {
    out << "bsphere, c (" << _center << "), r " << _radius;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::as_bounding_sphere
//       Access: Public, Virtual
//  Description: Virtual downcast method.  Returns this object as a
//               pointer of the indicated type, if it is in fact that
//               type.  Returns NULL if it is not that type.
////////////////////////////////////////////////////////////////////
const BoundingSphere *BoundingSphere::
as_bounding_sphere() const {
  return this;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::extend_other
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
extend_other(BoundingVolume *other) const {
  return other->extend_by_sphere(this);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::around_other
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
around_other(BoundingVolume *other,
             const BoundingVolume **first,
             const BoundingVolume **last) const {
  return other->around_spheres(first, last);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_other
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_other(const BoundingVolume *other) const {
  return other->contains_sphere(this);
}


////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::extend_by_point
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
extend_by_point(const LPoint3f &point) {
  nassertr(!point.is_nan(), false);

  if (is_empty()) {
    _center = point;
    _radius = 0.0f;
    _flags = 0;
  } else if (!is_infinite()) {
    LVector3f v = point - _center;
    float dist2 = dot(v, v);
    if (dist2 > _radius * _radius) {
      _radius = sqrtf(dist2);
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::extend_by_sphere
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
extend_by_sphere(const BoundingSphere *sphere) {
  nassertr(!sphere->is_empty() && !sphere->is_infinite(), false);
  nassertr(!is_infinite(), false);

  if (is_empty()) {
    _center = sphere->_center;
    _radius = sphere->_radius;
    _flags = 0;
  } else {
    float dist = length(sphere->_center - _center);

    _radius = max(_radius, dist + sphere->_radius);
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::extend_by_box
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
extend_by_box(const BoundingBox *box) {
  const LVector3f &min1 = box->get_minq();
  const LVector3f &max1 = box->get_maxq();

  if (is_empty()) {
    _center = (min1 + max1) * 0.5f;
    _radius = length(LVector3f(max1 - _center));
    _flags = 0;

  } else {
    // Find the minimum radius necessary to reach the corner.
    float max_dist2 = -1.0;
    for (int i = 0; i < 8; ++i) {
      float dist2 = (box->get_point(i) - _center).length_squared();
      if (dist2 > max_dist2) {
        max_dist2 = dist2;
      }
    }
    if (max_dist2 > _radius * _radius) {
      _radius = csqrt(max_dist2);
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::extend_by_hexahedron
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
extend_by_hexahedron(const BoundingHexahedron *hexahedron) {
  nassertr(!hexahedron->is_empty(), false);

  BoundingBox box(hexahedron->get_min(), hexahedron->get_max());
  box.local_object();
  return extend_by_box(&box);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::extend_by_finite
//       Access: Protected
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
extend_by_finite(const FiniteBoundingVolume *volume) {
  nassertr(!volume->is_empty(), false);

  BoundingBox box(volume->get_min(), volume->get_max());
  box.local_object();
  return extend_by_box(&box);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::around_points
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
around_points(const LPoint3f *first, const LPoint3f *last) {
  nassertr(first != last, false);

  // First, get the box of all the points to construct a bounding
  // box.
  const LPoint3f *p = first;

#ifndef NDEBUG
  // Skip any NaN points.
  int skipped_nan = 0;
  while (p != last && (*p).is_nan()) {
    ++p;
    ++skipped_nan;
  }
  if (p == last) {
    mathutil_cat.warning()
      << "BoundingSphere around NaN\n";
    return false;
  }
#endif

  LPoint3f min_box = *p;
  LPoint3f max_box = *p;
  ++p;

#ifndef NDEBUG
  // Skip more NaN points.
  while (p != last && (*p).is_nan()) {
    ++p;
    ++skipped_nan;
  }
#endif

  if (p == last) {
    // Only one point; we have a radius of zero.  This is not the same
    // thing as an empty sphere, because our volume contains one
    // point; an empty sphere contains no points.
    _center = min_box;
    _radius = 0.0f;

  } else {
    // More than one point; we have a nonzero radius.
    while (p != last) {
#ifndef NDEBUG
      // Skip more NaN points.
      if ((*p).is_nan()) {
        ++skipped_nan;
      } else
#endif
        {
          min_box.set(min(min_box[0], (*p)[0]),
                      min(min_box[1], (*p)[1]),
                      min(min_box[2], (*p)[2]));
          max_box.set(max(max_box[0], (*p)[0]),
                      max(max_box[1], (*p)[1]),
                      max(max_box[2], (*p)[2]));
        }
      ++p;
    }

    // Now take the center of the bounding box as the center of the sphere.
    _center = (min_box + max_box) * 0.5f;

    // Now walk back through to get the max distance from center.
    float max_dist2 = 0.0f;
    for (p = first; p != last; ++p) {
      LVector3f v = (*p) - _center;
      float dist2 = dot(v, v);
      max_dist2 = max(max_dist2, dist2);
    }

    _radius = sqrtf(max_dist2);
  }

#ifndef NDEBUG
  if (skipped_nan != 0) {
    mathutil_cat.warning()
      << "BoundingSphere ignored " << skipped_nan << " NaN points of "
      << (last - first) << " total.\n";
  }
#endif

  _flags = 0;

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::around_spheres
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
around_spheres(const BoundingVolume **first,
               const BoundingVolume **last) {
  return around_finite(first, last);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::around_boxes
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
around_boxes(const BoundingVolume **first,
             const BoundingVolume **last) {
  return around_finite(first, last);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::around_hexahedrons
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
around_hexahedrons(const BoundingVolume **first,
                   const BoundingVolume **last) {
  return around_finite(first, last);
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::around_finite
//       Access: Protected
//  Description: 
////////////////////////////////////////////////////////////////////
bool BoundingSphere::
around_finite(const BoundingVolume **first,
              const BoundingVolume **last) {
  nassertr(first != last, false);

  // We're given a set of bounding volumes, all of which are finite,
  // and at least the first one of which is guaranteed to be nonempty.
  // Some others may not be.

  // First, get the box of all the points to construct a bounding
  // box.
  const BoundingVolume **p = first;
  nassertr(!(*p)->is_empty() && !(*p)->is_infinite(), false);
  const FiniteBoundingVolume *vol = (*p)->as_finite_bounding_volume();
  nassertr(vol != (FiniteBoundingVolume *)NULL, false);
  LPoint3f min_box = vol->get_min();
  LPoint3f max_box = vol->get_max();

  bool any_spheres = (vol->as_bounding_sphere() != NULL);

  for (++p; p != last; ++p) {
    nassertr(!(*p)->is_infinite(), false);
    if (!(*p)->is_empty()) {
      vol = (*p)->as_finite_bounding_volume();
      if (vol == (FiniteBoundingVolume *)NULL) {
        set_infinite();
        return true;
      }
      LPoint3f min1 = vol->get_min();
      LPoint3f max1 = vol->get_max();
      min_box.set(min(min_box[0], min1[0]),
                  min(min_box[1], min1[1]),
                  min(min_box[2], min1[2]));
      max_box.set(max(max_box[0], max1[0]),
                  max(max_box[1], max1[1]),
                  max(max_box[2], max1[2]));

      if (vol->as_bounding_sphere() != NULL) {
        any_spheres = true;
      }
    }
  }

  // Now take the center of the bounding box as the center of the sphere.
  _center = (min_box + max_box) * 0.5f;

  if (!any_spheres) {
    // Since there are no spheres in the list, we have to make this
    // sphere fully enclose all of the bounding boxes.
    _radius = length(max_box - _center);

  } else {
    // We might be able to go tighter, by lopping off the corners of
    // the spheres.
    _radius = 0.0f;
    for (p = first; p != last; ++p) {
      if (!(*p)->is_empty()) {
        const BoundingSphere *sphere = (*p)->as_bounding_sphere();
        if (sphere != (BoundingSphere *)NULL) {
          // This is a sphere; consider its corner.
          float dist = length(sphere->_center - _center);
          _radius = max(_radius, dist + sphere->_radius);
          
        } else {
          // This is a nonsphere.  We fit around it.
          const FiniteBoundingVolume *vol = (*p)->as_finite_bounding_volume();
          nassertr(vol != (FiniteBoundingVolume *)NULL, false);

          BoundingBox box(vol->get_min(), vol->get_max());
          box.local_object();
          
          // Find the minimum radius necessary to reach the corner.
          float max_dist2 = -1.0;
          for (int i = 0; i < 8; ++i) {
            float dist2 = (box.get_point(i) - _center).length_squared();
            if (dist2 > max_dist2) {
              max_dist2 = dist2;
            }
          }
          _radius = max(_radius, csqrt(max_dist2));
        }
      }
    }
  }

  _flags = 0;
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_point
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_point(const LPoint3f &point) const {
  nassertr(!point.is_nan(), IF_no_intersection);

  if (is_empty()) {
    return IF_no_intersection;

  } else if (is_infinite()) {
    return IF_possible | IF_some | IF_all;

  } else {
    LVector3f v = point - _center;
    float dist2 = dot(v, v);
    return (dist2 <= _radius * _radius) ?
      IF_possible | IF_some | IF_all : IF_no_intersection;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_lineseg
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_lineseg(const LPoint3f &a, const LPoint3f &b) const {
  nassertr(!a.is_nan() && !b.is_nan(), IF_no_intersection);

  if (a == b) {
    return contains_point(a);
  }
  if (is_empty()) {
    return IF_no_intersection;

  } else if (is_infinite()) {
    return IF_possible | IF_some | IF_all;

  } else {
    LPoint3f from = a;
    LVector3f delta = b - a;
    float t1, t2;

    // Solve the equation for the intersection of a line with a sphere
    // using the quadratic equation.
    float A = dot(delta, delta);

    nassertr(A != 0.0f, 0);    // Trivial line segment.

    LVector3f fc = from - _center;
    float B = 2.0f * dot(delta, fc);
    float C = dot(fc, fc) - _radius * _radius;

    float radical = B*B - 4.0f*A*C;

    if (IS_NEARLY_ZERO(radical)) {
      // Tangent.
      t1 = t2 = -B / (2.0f*A);
      return (t1 >= 0.0f && t1 <= 1.0f) ?
                 IF_possible | IF_some : IF_no_intersection;
    }

    if (radical < 0.0f) {
      // No real roots: no intersection with the line.
      return IF_no_intersection;
    }

        float reciprocal_2A = 1.0f/(2.0f*A);
    float sqrt_radical = sqrtf(radical);

    t1 = ( -B - sqrt_radical ) * reciprocal_2A;
    t2 = ( -B + sqrt_radical ) * reciprocal_2A;

    if (t1 >= 0.0f && t2 <= 1.0f) {
      return IF_possible | IF_some | IF_all;
    } else if (t1 <= 1.0f && t2 >= 0.0f) {
      return IF_possible | IF_some;
    } else {
      return IF_no_intersection;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_sphere
//       Access: Protected, Virtual
//  Description: Double-dispatch support: called by contains_other()
//               when the type we're testing for intersection is known
//               to be a sphere.
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_sphere(const BoundingSphere *sphere) const {
  nassertr(!is_empty() && !is_infinite(), 0);
  nassertr(!sphere->is_empty() && !sphere->is_infinite(), 0);

  LVector3f v = sphere->_center - _center;
  float dist2 = dot(v, v);

  if (_radius >= sphere->_radius &&
      dist2 <= (_radius - sphere->_radius) * (_radius - sphere->_radius)) {
    // The other sphere is completely within this sphere.
    return IF_possible | IF_some | IF_all;

  } else if (dist2 > (_radius + sphere->_radius) * (_radius + sphere->_radius)) {
    // The other sphere is completely outside this sphere.
    return IF_no_intersection;

  } else {
    // The other sphere is partially within this sphere.
    return IF_possible | IF_some;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_box
//       Access: Protected, Virtual
//  Description: Double-dispatch support: called by contains_other()
//               when the type we're testing for intersection is known
//               to be a box.
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_box(const BoundingBox *box) const {
  return box->contains_sphere(this) & ~IF_all;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_hexahedron
//       Access: Protected, Virtual
//  Description: Double-dispatch support: called by contains_other()
//               when the type we're testing for intersection is known
//               to be a hexahedron.
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_hexahedron(const BoundingHexahedron *hexahedron) const {
  return hexahedron->contains_sphere(this) & ~IF_all;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_line
//       Access: Protected, Virtual
//  Description: Double-dispatch support: called by contains_other()
//               when the type we're testing for intersection is known
//               to be a line.
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_line(const BoundingLine *line) const {
  return line->contains_sphere(this) & ~IF_all;
}

////////////////////////////////////////////////////////////////////
//     Function: BoundingSphere::contains_plane
//       Access: Protected, Virtual
//  Description: Double-dispatch support: called by contains_other()
//               when the type we're testing for intersection is known
//               to be a plane.
////////////////////////////////////////////////////////////////////
int BoundingSphere::
contains_plane(const BoundingPlane *plane) const {
  return plane->contains_sphere(this) & ~IF_all;
}
